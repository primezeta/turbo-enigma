#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "ArchiveTransformMap.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::RegisteredTypeNames;

FArchive& operator<<(FArchive& Ar, FTransformMapFactory::ValueTypePtr& TransformMapPtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        TransformMapPtr = FTransformMapFactory::Create(TypeName);
        if (TransformMapPtr != nullptr)
        {
            Ar << *TransformMapPtr;
        }
        //else {//TODO: need to seek past the unregistered data?}
    }
    else if (TransformMapPtr != nullptr)
    {
        TypeName = UTF8_TO_TCHAR(TransformMapPtr->type().c_str());
        Ar << TypeName;
        Ar << *TransformMapPtr;
    }
    return Ar;
}

FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap)
{
    //De/serialize the tarray containing the transform map
    TArray<ANSICHAR> DataBytes;
    if (Ar.IsLoading())
    {
        DataBytes.BulkSerialize(Ar);
    }
    else
    {
        const FString TypeName = UTF8_TO_TCHAR(TransformMap.type().c_str());
        check(FTransformMapFactory::IsRegistered(TypeName));
        const int32 MapSize = VoxelDatabaseStatics::TransformMapStatics::SizeOfMap(TypeName);
        DataBytes.SetNumUninitialized(MapSize > 0 ? MapSize : 0, false);
    }

    const openvdb::Index32 DataArraySize = DataBytes.Num();
    check(DataArraySize > 0);

    //Create an i/o stream for reading/writing the tarray
    ANSICHAR* BufferStart = DataBytes.GetData();
    ANSICHAR* BufferEnd = DataBytes.GetData() + DataArraySize - 1;
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
