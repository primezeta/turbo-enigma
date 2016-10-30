#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabaseTransformMapTypeSpecifier.h"
#include "VoxelDatabaseTypeFactory.h"

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    virtual void Serialize(FArchive& Ar) override;
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap);

    template<typename MapType>
    static inline void RegisterTransformMapType()
    {
        static const FString TypeName = UTF8_TO_TCHAR(MapType::mapType().c_str());
        if (!IsRegistered(TypeName))
        {
            Register(TypeName, MapType::create);
            FTransformMapFactory::RegisteredTypeNames.Add(VoxelDatabaseTypeName<MapType>());
        }
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        return openvdb::math::MapRegistry::createMap(TCHAR_TO_UTF8(*TypeName));
    }

    static ValueTypePtr Create(const FVoxelDatabaseTransformMapTypeSpecifier& TransformMapTypeSpecifier)
    {
        const FString TypeName = TransformMapTypeSpecifier.GetTypeName();
        return Create(TypeName);
    }

    static void Register(const FString& TypeName, openvdb::math::MapBase::MapFactory Factory)
    {
        openvdb::math::MapRegistry::registerMap(TCHAR_TO_UTF8(*TypeName), Factory);
    }

    static void Unregister(const FString& TypeName)
    {
        openvdb::math::MapRegistry::unregisterMap(TCHAR_TO_UTF8(*TypeName));
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::math::MapRegistry::isRegistered(TCHAR_TO_UTF8(*TypeName));
    }

    static void ClearRegistry()
    {
        openvdb::math::MapRegistry::clear();
    }
};

FORCEINLINE static FArchive& operator<<(FArchive& Ar, FTransformMapFactory::ValueTypePtr& TransformMapPtr)
{
    if (TransformMapPtr != nullptr)
    {
        Ar << *TransformMapPtr;
    }
    return Ar;
}
