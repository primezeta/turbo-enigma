#pragma once
#include "EngineGridTypes.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FMetaValueFactory::ValueTypePtr& MetaValuePtr);
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue);

    template<typename MetaType>
    static inline void RegisterMetaType()
    {
        const openvdb::Name TypeName = openvdb::TypedMetadata<MetaType>::staticTypeName().c_str();
        if (!openvdb::Metadata::isRegisteredType(TypeName))
        {
            try
            {
                openvdb::Metadata::registerType(TypeName, openvdb::TypedMetadata<MetaType>::createMetadata);
                FMetaValueFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(openvdb::typeNameAsString<MetaType>()));
            }
            catch (const openvdb::KeyError& e)
            {
                (void)e; //TODO log error (metadata typename already registered)
            }
        }
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::Metadata::isRegisteredType(TCHAR_TO_UTF8(*TypeName));
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        ValueTypePtr MetaValuePtr = nullptr;
        try
        {
            MetaValuePtr = openvdb::Metadata::createMetadata(TCHAR_TO_UTF8(*TypeName));
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (metadata type not registered)
        }
        return MetaValuePtr;
    }

    static void ClearRegistry()
    {
        openvdb::Metadata::clearRegistry();
    }
};

#pragma warning(pop)
