#pragma once
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabaseTypeFactory.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, FMetaValueFactory::ValueTypePtr& MetaValuePtr);
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue);
    VOXELMATEINLINE bool Serialize(FArchive& Ar)
    {
        Ar << ValuePtr;
        return true;
    }

    template<typename MetaType>
    static void Register()
    {
        try
        {
            openvdb::TypedMetadata<MetaType>::registerType();
            FMetaValueFactory::RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<MetaType::ValueType>()), false));
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (metadata type already registered)
        }
    }

    template<typename MetaType>
    static bool IsRegistered()
    {
        return openvdb::TypedMetadata<MetaType>::isRegisteredType();
    }

    template<typename MetaType>
    static ValueTypePtr Create()
    {
        ValueTypePtr MetaValuePtr = nullptr;
        try
        {
            MetaValuePtr = openvdb::TypedMetadata<MetaType>::createMetadata();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (metadata type is not registered)
        }
        check(MetaValuePtr != nullptr);
        return MetaValuePtr;
    }
};

#pragma warning(pop)
