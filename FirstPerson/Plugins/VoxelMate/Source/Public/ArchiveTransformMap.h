#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabaseTypeFactory.h"

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap);
    virtual void Serialize(FArchive& Ar) override;

    template<typename MapType>
    static inline void RegisterTransformMapType()
    {
        if (!IsRegistered(MapType::mapType()))
        {
            Register(MapType::mapType(), MapType::create);
            FTransformMapFactory::RegisteredTypeDisplayNames.Add(VoxelDatabaseTypeNameDisplay<MapType>());
        }
    }

    static ValueTypePtr Create(const openvdb::Name& TypeName)
    {
        return openvdb::math::MapRegistry::createMap(TypeName);
    }

    static void Register(const openvdb::Name& TypeName, openvdb::math::MapBase::MapFactory Factory)
    {
        openvdb::math::MapRegistry::registerMap(TypeName, Factory);
    }

    static void Unregister(const openvdb::Name& TypeName)
    {
        openvdb::math::MapRegistry::unregisterMap(TypeName);
    }

    static bool IsRegistered(const openvdb::Name& TypeName)
    {
        return openvdb::math::MapRegistry::isRegistered(TypeName);
    }

    static void ClearRegistry()
    {
        openvdb::math::MapRegistry::clear();
    }
};
