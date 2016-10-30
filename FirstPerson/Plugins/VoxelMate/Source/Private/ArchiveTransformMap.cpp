#include "VoxelMatePrivatePCH.h"
#include "ArchiveTransformMap.h"

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::RegisteredTypeNames;

void FTransformMapFactory::Serialize(FArchive& Ar)
{
    if (Ar.IsLoading())
    {
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(Type);
        ValuePtr = FTransformMapFactory::Create(TypeName);
    }

    check(ValuePtr != nullptr);
    openvdb::math::MapBase& Value = *ValuePtr;
    Ar << Value;
}

FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap)
{
    //De/serialize the tarray containing the transform map
    TArray<char> DataBytes;
    if (Ar.IsLoading())
    {
        DataBytes.BulkSerialize(Ar);
    }
    else
    {
        const FString TypeName = UTF8_TO_TCHAR(TransformMap.type().c_str());
        const int32 MapSize = VoxelDatabaseStatics::TransformMapStatics::SizeOfMap(TypeName);
        DataBytes.SetNumUninitialized(MapSize > 0 ? MapSize : 0, false);
    }

    const openvdb::Index32 DataArraySize = DataBytes.Num();
    check(DataArraySize > 0);

    //Create an i/o stream for reading/writing the tarray
    char* BufferStart = DataBytes.GetData();
    char* BufferEnd = DataBytes.GetData() + DataArraySize - 1;
    boost::iostreams::array BufferDevice(BufferStart, BufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    //TODO any exceptions to handle?
    IOStream.open(BufferDevice);
    if (Ar.IsLoading())
    {
        TransformMap.read(IOStream);
    }
    else
    {
        TransformMap.write(IOStream);
        DataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
