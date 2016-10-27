#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseTypeFactory.h"

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue);
    virtual void Serialize(FArchive& Ar) override;

    template<typename MetaType>
    static inline void RegisterMetaType()
    {
        if (!IsRegistered(openvdb::TypedMetadata<MetaType>::staticTypeName()))
        {
            Register(openvdb::TypedMetadata<MetaType>::staticTypeName(), openvdb::TypedMetadata<MetaType>::createMetadata);
            FMetaValueFactory::RegisteredTypeDisplayNames.Add(VoxelDatabaseTypeNameDisplay<MetaType>());
        }
    }

    static ValueTypePtr Create(const openvdb::Name& TypeName)
    {
        return openvdb::Metadata::createMetadata(TypeName);
    }

    static void Register(const openvdb::Name& TypeName, openvdb::Metadata::Ptr(*createMetadata)())
    {
        openvdb::Metadata::registerType(TypeName, createMetadata);
    }

    static void Unregister(const openvdb::Name& TypeName)
    {
        openvdb::Metadata::unregisterType(TypeName);
    }

    static bool IsRegistered(const openvdb::Name& TypeName)
    {
        return openvdb::Metadata::isRegisteredType(TypeName);
    }

    static void ClearRegistry()
    {
        openvdb::Metadata::clearRegistry();
    }
};
