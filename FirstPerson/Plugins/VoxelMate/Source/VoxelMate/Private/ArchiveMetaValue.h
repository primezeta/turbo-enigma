#pragma once
#include "VoxelDatabaseTypeFactory.h"
#include "openvdb/openvdb.h"

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    template<typename MetaType>
    static void Register()
    {
        try
        {
            openvdb::TypedMetadata<MetaType>::registerType();
            FMetaValueFactory::RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<MetaType>()), false));
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
