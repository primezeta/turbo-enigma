#include "VoxelMatePCH.h"
#include "ArchiveTransformMap.h"
#include "EngineGridTypes.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

bool FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::IsInitialized = false;
TArray<FString> FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::RegisteredTypeNames;

bool FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::IsRegisteredType(const FString& TypeName)
{
    return openvdb::math::MapRegistry::isRegistered(TCHAR_TO_UTF8(*TypeName));
}

FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::ValueTypePtr FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::CreateType(const FString& TypeName)
{
    FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::ValueTypePtr TransformMapPtr = nullptr;
    try
    {
        TransformMapPtr = openvdb::math::MapRegistry::createMap(TCHAR_TO_UTF8(*TypeName));
    }
    catch (const openvdb::LookupError& e)
    {
        (void)e; //TODO log error (transform map type not registered)
    }
    return  TransformMapPtr;
}

void FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::RegisterSupportedTypes()
{
    if (FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::IsInitialized)
    {
        return;
    }
    openvdb::math::MapRegistry::clear();
    FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::RegisteredTypeNames.Empty();
    FTransformMapFactory::Register<FAffineCoordinateTransform>();
    FTransformMapFactory::Register<FUnitaryCoordinateTransform>();
    FTransformMapFactory::Register<FScaleCoordinateTransform>();
    FTransformMapFactory::Register<FUniformScaleCoordinateTransform>();
    FTransformMapFactory::Register<FTranslationCoordinateTransform>();
    FTransformMapFactory::Register<FScaleTranslationCoordinateTransform>();
    FTransformMapFactory::Register<FUniformScaleTranslationCoordinateTransform>();
    FTransformMapFactory::Register<FNonlinearFrustumCoordinateTransform>();
    FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::IsInitialized = true;
}

void FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::UnregisterSupportedTypes()
{
    openvdb::math::MapRegistry::clear();
    FVoxelDatabaseTypeFactory<openvdb::math::MapBase>::IsInitialized = false;
}

FArchive& operator<<(FArchive& Ar, FTransformMapFactory::ValueTypePtr& TransformMapPtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        TransformMapPtr = FTransformMapFactory::CreateType(TypeName);
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
