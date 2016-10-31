#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include "VoxelDatabaseGridTypeSpecifier.h"
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>

struct FGridFactory : public FVoxelDatabaseTypeFactory<openvdb::GridBase>
{
    static TMap<FString, int32> TopologySizeByTreeType;
    uint8 CompressionFlags;

    virtual void Serialize(FArchive& Ar) override;
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid);

    template<typename TreeType>
    static void RegisterGridType()
    {
        const FString TypeName = VoxelDatabaseTypeName<TreeType::ValueType>();
        if (!FGridFactory::IsRegistered(TypeName))
        {
            Register(TypeName, VoxelDatabaseStatics::GridStatics::Factory<TreeType>);
            FGridFactory::RegisteredTypeNames.Add(TypeName);
        }
    }

    static openvdb::GridBase::Ptr Create(const FString& TypeName)
    {
        bool IsGridAdded = false;
        ValueTypePtr GridPtr;
        try
        {
            GridPtr = openvdb::GridBase::createGrid(TCHAR_TO_UTF8(*TypeName));
        }
        catch (const openvdb::LookupError& e)
        {
            //TODO error handling (grid type is not registered)
            (void)e;
        }
        return GridPtr;
    }

    static openvdb::GridBase::Ptr Create(const UVoxelDatabaseGridTypeSpecifier& GridTypeSpecifier)
    {
        const FString TypeName = GridTypeSpecifier.GetTypeName();
        return Create(TypeName);
    }

    /* Register a grid containing voxels of DataType with tree topology Root, Internal, and Leaf
    */
    template<typename DataType>
    static inline void RegisterGrid3()
    {
        RegisterGridType<FTree3<DataType>>();
    }

    /* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, and Leaf
    */
    template<typename DataType>
    static inline void RegisterGrid4()
    {
        RegisterGridType<FTree4<DataType>>();
    }

    /* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, Internal2, and Leaf
    */
    template<typename DataType>
    static inline void RegisterGrid5()
    {
        RegisterGridType<FTree5<DataType>>();
    }

    /* Register a standard grid type (i.e. Grid4)
    */
    template<typename DataType>
    static inline void RegisterStandardGrid()
    {
        RegisterGrid4<DataType>();
    }

    static void Register(const FString& TypeName, openvdb::GridBase::GridFactory Factory)
    {
        VoxelDatabaseStatics::GridStatics::RegisterGrid(TypeName, Factory);
    }

    static void Unregister(const FString& TypeName)
    {
        VoxelDatabaseStatics::GridStatics::UnregisterGrid(TypeName);
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::GridBase::isRegistered(TCHAR_TO_UTF8(*TypeName));
    }

    static void ClearRegistry()
    {
        openvdb::GridBase::clearRegistry();
    }
};

FORCEINLINE static FArchive& operator<<(FArchive& Ar, FGridFactory::ValueTypePtr& GridPtr)
{
    if (GridPtr != nullptr)
    {
        Ar << *GridPtr;
    }
    return Ar;
}
