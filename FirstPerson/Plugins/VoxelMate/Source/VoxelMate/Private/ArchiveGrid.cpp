#include "VoxelMatePCH.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeNames;
TMap<FString, int32> FGridFactory::CachedTopologySizeByTreeType;

FArchive& operator<<(FArchive& Ar, FGridFactory::ValueTypePtr& GridPtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        GridPtr = FGridFactory::Create(TypeName);
        if (GridPtr != nullptr)
        {
            Ar << *GridPtr;
        }
        //else {//TODO: need to seek past the unregistered data?}
    }
    else if (GridPtr != nullptr)
    {
        TypeName = UTF8_TO_TCHAR(GridPtr->type().c_str());
        Ar << TypeName;
        Ar << *GridPtr;
    }
    return Ar;
}

FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid)
{
    const bool IsLoading = Ar.IsLoading();

    //TODO read grid name then set it if version < OPENVDB_FILE_VERSION_NO_GRIDMAP because older versions do not store grid name as metadata
    //De/serialize the grid metadata
    openvdb::MetaMap& GridMetaMap = Grid;
    Ar << GridMetaMap;

    //De/serialize the grid map and give it a transform
    FTransformMapFactory TransformMapFactory;
    if (IsLoading)
    {
        TransformMapFactory.Serialize(Ar);
        openvdb::math::Transform::Ptr TransformPtr = openvdb::math::Transform::Ptr(new openvdb::math::Transform(TransformMapFactory.ValuePtr));
        Grid.setTransform(TransformPtr);
    }
    else
    {
        TransformMapFactory.ValuePtr = Grid.transform().baseMap();
        TransformMapFactory.Serialize(Ar);
    }

    bool FloatSavedAsHalf = Grid.saveFloatAsHalf();
    Ar << FloatSavedAsHalf;
    if (IsLoading)
    {
        Grid.setSaveFloatAsHalf(FloatSavedAsHalf);
    }

    const openvdb::TreeBase& Tree = Grid.baseTree();
    const openvdb::Name& TreeType = Grid.baseTree().type();

    //Save the topology byte count for each tree type so that we don't have to keep calculating it over and over for many trees
    int32& TopologyByteCount = FGridFactory::CachedTopologySizeByTreeType.FindOrAdd(UTF8_TO_TCHAR(TreeType.c_str()));

    //Peek at the byte count of the array (if saving and the size is currently unknown, write an initial 0)
    const int64 TopologyByteCountPos = Ar.Tell();
    Ar << TopologyByteCount;

    if (TopologyByteCount == 0)
    {
        check(!IsLoading); //If loading but the topology byte count was 0 then something went wrong while saving

        //Calculate the unknown topology size by writing to a back_inserter
        std::vector<ANSICHAR> TopologyBytes;
        boost::iostreams::back_insert_device<std::vector<ANSICHAR>> TopologySink(TopologyBytes);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::vector<ANSICHAR>>> TempTopologyStream;

        ////Estimate the topology size as double the memory usage, then reserve that amount to reduce the chance of vector reallocation
        //const size_t TopologySizeEstimate = Grid.memUsage() * 2;
        //TopologyBytes.reserve(TopologySizeEstimate);

        //Write the topology to the temporary stream in order to determine the topology byte count
        TempTopologyStream.open(TopologySink);
        Grid.writeTopology(static_cast<OutputStreamType&>(TempTopologyStream));
        TempTopologyStream.close();

        const size_t ByteCount = TopologyBytes.size();
        TopologyByteCount = ByteCount;
    }

    TArray<ANSICHAR> TopologyDataBytes;
    if (IsLoading)
    {
        TopologyDataBytes.BulkSerialize(Ar);
        check(TopologyByteCount == TopologyDataBytes.Num());
    }
    else
    {
        TopologyDataBytes.SetNumUninitialized(TopologyByteCount);
    }

    ANSICHAR* TopologyBufferStart = TopologyDataBytes.GetData();
    ANSICHAR* TopologyBufferEnd = TopologyDataBytes.GetData() + TopologyByteCount - 1;
    boost::iostreams::array TopologyBufferDevice(TopologyBufferStart, TopologyBufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    IOStream.open(TopologyBufferDevice);
    if (Ar.IsLoading())
    {
        Grid.readTopology(static_cast<std::istream&>(IOStream));
        //TODO if version >= OPENVDB_FILE_VERSION_GRID_INSTANCING
            //grid->readTransform(is);
            //if (!gd.isInstance()) {
            //    grid->readTopology(is);
            //    Local::readBuffers(*grid, is, bbox);
            //}
        //else
            //grid->readTopology(is);
            //grid->readTransform(is);
    }
    else
    {
        Grid.writeTopology(static_cast<OutputStreamType&>(IOStream));
        TopologyDataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
