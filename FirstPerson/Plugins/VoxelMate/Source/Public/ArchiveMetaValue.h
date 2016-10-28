#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseTypeFactory.h"

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    virtual void Serialize(FArchive& Ar) override;
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue);

    template<typename MetaType>
    static inline void RegisterMetaType()
    {
        static const FString TypeName = UTF8_TO_TCHAR(openvdb::TypedMetadata<MetaType>::staticTypeName().c_str());
        if (!IsRegistered(TypeName))
        {
            Register(TypeName, openvdb::TypedMetadata<MetaType>::createMetadata);
            FMetaValueFactory::RegisteredTypeNames.Add(VoxelDatabaseTypeName<MetaType>());
        }
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        return openvdb::Metadata::createMetadata(TCHAR_TO_UTF8(*TypeName));
    }

    static void Register(const FString& TypeName, openvdb::Metadata::Ptr(*createMetadata)())
    {
        openvdb::Metadata::registerType(TCHAR_TO_UTF8(*TypeName), createMetadata);
    }

    static void Unregister(const FString& TypeName)
    {
        openvdb::Metadata::unregisterType(TCHAR_TO_UTF8(*TypeName));
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::Metadata::isRegisteredType(TCHAR_TO_UTF8(*TypeName));
    }

    static void ClearRegistry()
    {
        openvdb::Metadata::clearRegistry();
    }
};

FORCEINLINE static FArchive& operator<<(FArchive& Ar, FMetaValueFactory::ValueTypePtr& MetaValuePtr)
{
    if (MetaValuePtr != nullptr)
    {
        Ar << *MetaValuePtr;
    }
    return Ar;
}
