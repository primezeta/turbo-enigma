#include "VoxelMatePCH.h"
#include "GridTypes.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>

bool FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = false;
TArray<FString> FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeNames;

bool FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsRegisteredType(const FString& TypeName)
{
    return openvdb::GridBase::isRegistered(TCHAR_TO_UTF8(*TypeName));
}

FVoxelDatabaseTypeFactory<openvdb::GridBase>::ValueTypePtr FVoxelDatabaseTypeFactory<openvdb::GridBase>::CreateType(const FString& TypeName)
{
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::ValueTypePtr GridPtr = nullptr;
    try
    {
        GridPtr = openvdb::GridBase::createGrid(TCHAR_TO_UTF8(*TypeName));
    }
    catch (const openvdb::LookupError& e)
    {
        (void)e; //TODO log error (grid type name is not registered)
    }
    return GridPtr;
}

void FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisterSupportedTypes()
{
    if (FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized)
    {
        return;
    }
    openvdb::GridBase::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeNames.Empty();
    FGridFactory::Register<FVoxelDatabaseBoolVoxel>();
    FGridFactory::Register<FVoxelDatabaseFloatVoxel>();
    FGridFactory::Register<FVoxelDatabaseDoubleVoxel>();
    FGridFactory::Register<FVoxelDatabaseUInt8Voxel>();
    FGridFactory::Register<FVoxelDatabaseUInt16Voxel>();
    FGridFactory::Register<FVoxelDatabaseUInt32Voxel>();
    FGridFactory::Register<FVoxelDatabaseUInt64Voxel>();
    FGridFactory::Register<FVoxelDatabaseInt8Voxel>();
    FGridFactory::Register<FVoxelDatabaseInt16Voxel>();
    FGridFactory::Register<FVoxelDatabaseInt32Voxel>();
    FGridFactory::Register<FVoxelDatabaseInt64Voxel>();
    FGridFactory::Register<FVoxelDatabaseVectorVoxel>();
    FGridFactory::Register<FVoxelDatabaseVector4Voxel>();
    FGridFactory::Register<FVoxelDatabaseVector2DVoxel>();
    FGridFactory::Register<FVoxelDatabaseColorVoxel>();
    FGridFactory::Register<FVoxelDatabaseLinearColorVoxel>();
    FGridFactory::Register<FVoxelDatabasePackedNormalVoxel>();
    FGridFactory::Register<FVoxelDatabasePackedRGB10A2NVoxel>();
    FGridFactory::Register<FVoxelDatabasePackedRGBA16NVoxel>();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = true;
}

void FVoxelDatabaseTypeFactory<openvdb::GridBase>::UnregisterSupportedTypes()
{
    openvdb::GridBase::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = false;
}

FArchive& operator<<(FArchive& Ar, FGridFactory::ValueTypePtr& GridPtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        GridPtr = FGridFactory::CreateType(TypeName);
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

    int64 TopologyByteCount = 0;
    TArray<ANSICHAR> TopologyDataBytes;

    if (Ar.IsLoading())
    {
        TopologyDataBytes.BulkSerialize(Ar);
        TopologyByteCount = (int64)TopologyDataBytes.Num();
    }
    else
    {
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
        TopologyByteCount = (int64)TopologyBytes.size();
        TopologyDataBytes.SetNumUninitialized((int32)TopologyByteCount);
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
