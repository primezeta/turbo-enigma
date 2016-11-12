#pragma once
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)
#include <openvdb/math/Maps.h>

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FTransformMapFactory::ValueTypePtr& TransformMapPtr);
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap);

    template<typename MapType>
    static inline void RegisterTransformMapType()
    {
        if (!MapType::isRegistered())
        {
            //Translate the type name from the openvdb map type to the UE4 map type
            const FString MapTypeName = GridIOStatics::GetMapTypeName(MapType::mapType());
            if (!MapTypeName.IsEmpty())
            {
                MapType::registerMap();
                FTransformMapFactory::RegisteredTypeNames.Add(MapTypeName);
            }
        }
    }

    static bool IsRegistered(const FString& TypeName)
    {
        bool TypeIsRegistered = false;
        const openvdb::Name MapTypeName = GridIOStatics::GetMapTypeName(TypeName);
        if (!MapTypeName.empty())
        {
            TypeIsRegistered = openvdb::math::MapRegistry::isRegistered(MapTypeName);
        }
        return TypeIsRegistered;
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        //Translate the type name from the UE4 map type to the openvdb map type
        ValueTypePtr TransformMapPtr = nullptr;
        try
        {
            const openvdb::Name MapTypeName = GridIOStatics::GetMapTypeName(TypeName);
            if (!MapTypeName.empty())
            {
                TransformMapPtr = openvdb::math::MapRegistry::createMap(MapTypeName);
            }
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
