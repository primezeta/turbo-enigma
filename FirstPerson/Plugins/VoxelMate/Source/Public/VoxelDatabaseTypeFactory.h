#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"

#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <openvdb/io/io.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

template<typename OpenVdbType>
struct FVoxelDatabaseTypeFactory
{
    typedef typename OpenVdbType ValueType;
    typedef typename boost::shared_ptr<ValueType> ValueTypePtr;

    static TArray<FString> RegisteredTypeNames;
    static TArray<FString> RegisteredTypeDisplayNames;
    EVoxelDatabaseType Type;
    ValueTypePtr ValuePtr;

    FVoxelDatabaseTypeFactory()
    {}

    FVoxelDatabaseTypeFactory(const UVoxelDatabaseMetadataTypeSpecifier& TypeSpecifier)
        : TypeName(Type)
    {
        bool IsGridAdded = false;
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(TypeSpecifier.Type);
        FVoxelDatabaseTypeFactory::Register(TypeName);
    }

    virtual inline bool IsValid()
    {
        return ValuePtr != nullptr;
    }

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoxelDatabaseTypeFactory& Factory)
    {
        Factory.Serialize(Ar);
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) = 0;
};
