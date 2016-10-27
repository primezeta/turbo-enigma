#include "VoxelMatePrivatePCH.h"
#include "ArchiveGrid.h"

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeDisplayNames;
TMap<FString, int32> FGridFactory::TopologySizeByTreeType;

void FGridFactory::Serialize(FArchive& Ar)
{
    if (Ar.IsLoading())
    {
        ValuePtr = FGridFactory::Create(TypeName);
    }

    check(ValuePtr != nullptr);
    openvdb::GridBase& Value = *ValuePtr;
    Ar << Value;
}

FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid)
{
    const bool IsLoading = Ar.IsLoading();
    ////TODO where to set the compression flags?
    //if (IsLoading)
    //{
    //    //Read and validate the grid compression flag
    //    Ar << CompressionFlags;
    //    if (!GridCompressionFlagsAreInRange(GridFactory.CompressionFlags))
    //    {
    //        GridFactory.CompressionFlags = 0;
    //        //TODO log error (read invalid grid compression flag)
    //    }
    //}
    //else
    //{
    //    //Validate and write the grid compression flag
    //    if (!GridCompressionFlagsAreInRange(GridFactory.CompressionFlags))
    //    {
    //        GridFactory.CompressionFlags = 0;
    //        //TODO log error (tried to write invalid grid compression flag)
    //    }
    //    Ar << GridFactory.CompressionFlags;
    //}

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

    int32 FloatSavedAsHalf = (int32)Grid.saveFloatAsHalf();
    Ar << FloatSavedAsHalf;
    if (IsLoading)
    {
        Grid.setSaveFloatAsHalf((bool)FloatSavedAsHalf);
    }

    const openvdb::TreeBase& Tree = Grid.baseTree();
    const openvdb::Name& TreeType = Grid.baseTree().type();

    //Peek at the byte count of the array (if saving and the size is currently unknown, write an initial 0)
    int32& TopologyByteCount = FGridFactory::TopologySizeByTreeType.FindOrAdd(UTF8_TO_TCHAR(TreeType.c_str()));
    const int64 TopologyByteCountPos = Ar.Tell();
    Ar << TopologyByteCount;
    Ar.Seek(TopologyByteCountPos);

    if (TopologyByteCount == 0)
    {
        check(!IsLoading); //If loading but the topology byte count was 0 then something went wrong while writing

                           //Calculate the unknown topology size by writing to a back_inserter
        std::vector<char> TopologyBytes;
        boost::iostreams::back_insert_device<std::vector<char>> TopologySink(TopologyBytes);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::vector<char>>> TopologyStream(TopologySink);

        const size_t TopologySizeEstimate = Grid.memUsage() * 2;
        TopologyBytes.reserve(TopologySizeEstimate);
        Grid.writeTopology(TopologyStream);

        const size_t ByteCount = TopologyBytes.size();
        check(ByteCount > 0 && ByteCount <= INT32_MAX);
        TopologyByteCount = ByteCount;
    }

    TArray<char> TopologyDataBytes;
    if (IsLoading)
    {
        TopologyDataBytes.BulkSerialize(Ar);
        check(TopologyByteCount == TopologyDataBytes.Num());
    }
    else
    {
        TopologyDataBytes.SetNumUninitialized(TopologyByteCount);
    }

    char* TopologyBufferStart = TopologyDataBytes.GetData();
    char* TopologyBufferEnd = TopologyDataBytes.GetData() + TopologyByteCount - 1;
    boost::iostreams::array TopologyBufferDevice(TopologyBufferStart, TopologyBufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    IOStream.open(TopologyBufferDevice);
    if (Ar.IsLoading())
    {
        Grid.readTopology(IOStream);
    }
    else
    {
        Grid.writeTopology(IOStream);
        TopologyDataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
