#pragma once
#include "EngineGridTypes.h"
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
    static void Register()
    {
        try
        {
            TTransformMapTypeTranslator<MapType>::Type::registerMap();
            FTransformMapFactory::RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<MapType>()), false));
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (map type already registered)
        }
    }

    template<typename MapType>
    static bool IsRegistered()
    {
        return TTransformMapTypeTranslator<MapType>::Type::isRegistered();
    }

    template<typename MapType>
    static ValueTypePtr Create()
    {
        ValueTypePtr TransformMapPtr = nullptr;
        try
        {
            TransformMapPtr = TTransformMapTypeTranslator<MapType>::Type::create();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (transform map type not registered)
        }
        return  TransformMapPtr;
    }
};

#pragma warning(pop)
