#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>

struct FGridFactory : public FVoxelDatabaseTypeFactory<openvdb::GridBase>
{
    static TMap<FString, int32> TopologySizeByTreeType;
    uint8 CompressionFlags;

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid);
    virtual void Serialize(FArchive& Ar) override;

    template<typename TreeType>
    static void RegisterGridType()
    {
        TreeType Tree;
        const openvdb::Name& Type = Tree.type();
        if (!FGridFactory::IsRegistered(Type))
        {
            Register(Type, Create<TreeType>);
            FGridFactory::RegisteredTypeDisplayNames.Add(VoxelDatabaseTypeNameDisplay<TreeType::ValueType>());
        }
    }

    template<typename TreeType>
    static openvdb::GridBase::Ptr Create()
    {
        return openvdb::Grid<TreeType>::create();
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

    static ValueTypePtr Create(const openvdb::Name& TypeName)
    {
        return openvdb::GridBase::createGrid(TypeName);
    }

    static void Register(const openvdb::Name& TypeName, openvdb::GridBase::GridFactory Factory)
    {
        VoxelDatabaseStatics::GridStatics::RegisterGrid(TypeName, Factory);
    }

    static void Unregister(const openvdb::Name& TypeName)
    {
        VoxelDatabaseStatics::GridStatics::UnregisterGrid(TypeName);
    }

    static bool IsRegistered(const openvdb::Name& TypeName)
    {
        return openvdb::GridBase::isRegistered(TypeName);
    }

    static void ClearRegistry()
    {
        openvdb::GridBase::clearRegistry();
    }
};
