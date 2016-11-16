#pragma once
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabaseStatics.h"
#include "VoxelDatabaseTypeFactory.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)
#include <openvdb/math/Maps.h>

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, FTransformMapFactory::ValueTypePtr& TransformMapPtr);
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap);
    VOXELMATEINLINE bool Serialize(FArchive& Ar)
    {
        Ar << ValuePtr;
        return true;
    }

    template<typename MapType>
    static inline void RegisterTransformMapType()
    {
        try
        {
            MapType::registerMap();
            const openvdb::Name& MapTypeName = MapType::mapType();
            if (MapTypeName == openvdb::math::AffineMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FAffineMap"));
            }
            else if (MapTypeName == openvdb::math::UnitaryMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FUnitaryMap"));
            }
            else if (MapTypeName == openvdb::math::ScaleMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FScaleMap"));
            }
            else if (MapTypeName == openvdb::math::UniformScaleMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FUniformScaleMap"));
            }
            else if (MapTypeName == openvdb::math::TranslationMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FTranslationMap"));
            }
            else if (MapTypeName == openvdb::math::ScaleTranslateMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FScaleTranslationMap"));
            }
            else if (MapTypeName == openvdb::math::UniformScaleTranslateMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FUniformScaleTranslationMap"));
            }
            else if (MapTypeName == openvdb::math::NonlinearFrustumMap::mapType())
            {
                FTransformMapFactory::RegisteredTypeNames.Add(TEXT("FNonlinearFrustumMap"));
            }
            else
            {
                check(false);
            }
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (map type already registered)
        }
    }

    static bool IsRegistered(const FString& TypeName)
    {
        bool TypeIsRegistered = false;
        TypeIsRegistered = openvdb::math::MapRegistry::isRegistered(TCHAR_TO_UTF8(*TypeName));
        return TypeIsRegistered;
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        //Translate the type name from the UE4 map type to the openvdb map type
        ValueTypePtr TransformMapPtr = nullptr;
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

    static void ClearRegistry()
    {
        openvdb::math::MapRegistry::clear();
    }
};

#pragma warning(pop)
