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
        const openvdb::Name& MapTypeName = TransformMapPtr->type();
        if (MapTypeName == openvdb::math::AffineMap::mapType())
        {
            TypeName = TEXT("FAffineMap");
        }
        else if (MapTypeName == openvdb::math::UnitaryMap::mapType())
        {
            TypeName = TEXT("FUnitaryMap");
        }
        else if (MapTypeName == openvdb::math::ScaleMap::mapType())
        {
            TypeName = TEXT("FScaleMap");
        }
        else if (MapTypeName == openvdb::math::UniformScaleMap::mapType())
        {
            TypeName = TEXT("FUniformScaleMap");
        }
        else if (MapTypeName == openvdb::math::TranslationMap::mapType())
        {
            TypeName = TEXT("FTranslationMap");
        }
        else if (MapTypeName == openvdb::math::ScaleTranslateMap::mapType())
        {
            TypeName = TEXT("FScaleTranslationMap");
        }
        else if (MapTypeName == openvdb::math::UniformScaleTranslateMap::mapType())
        {
            TypeName = TEXT("FUniformScaleTranslationMap");
        }
        else if (MapTypeName == openvdb::math::NonlinearFrustumMap::mapType())
        {
            TypeName = TEXT("FNonlinearFrustumMap");
        }
        else
        {
            check(false);
        }
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
        const int32 MapSize = VoxelDatabaseStatics::TransformMapStatics::SizeOfMap(TransformMap.type());
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
