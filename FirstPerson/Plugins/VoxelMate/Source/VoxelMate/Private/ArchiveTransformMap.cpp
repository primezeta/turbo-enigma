#include "VoxelMatePCH.h"
#include "ArchiveTransformMap.h"
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
