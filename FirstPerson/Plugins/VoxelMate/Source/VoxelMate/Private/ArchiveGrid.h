#pragma once
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabaseTypeFactory.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

struct FGridFactory : public FVoxelDatabaseTypeFactory<openvdb::GridBase>
{
    static TMap<FString, int32> CachedTopologySizeByTreeType;
    uint8 CompressionFlags;

    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, FGridFactory::ValueTypePtr& GridPtr);
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid);
    VOXELMATEINLINE bool Serialize(FArchive& Ar)
    {
        Ar << ValuePtr;
        return true;
    }

    template<typename VoxelType>
    static void Register()
    {
        try
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::registerGrid();
            RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<VoxelType::ValueType>()), false));
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (grid type already registered)
        }
    }

    template<typename VoxelType>
    static bool IsRegistered()
    {
        return openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::isRegistered();
    }

    template<typename VoxelType>
    static ValueTypePtr Create()
    {
        ValueTypePtr GridPtr = nullptr;
        try
        {
            GridPtr = openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::create();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (grid type name is not registered)
        }
        check(GridPtr != nullptr);
        return GridPtr;
    }

    template<typename VoxelType>
    static typename openvdb::Grid<typename openvdb::tree::Tree4<VoxelType>::Type>::Ptr ShallowCopyGrid(ValueTypeConstPtr& InGridPtr)
    {
        typename openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr GridPtr = nullptr;
        if (InGridPtr != nullptr)
        {
            if (InGridPtr->isType<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>())
            {
                GridPtr = openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>(*InGridPtr, openvdb::ShallowCopy);
                check(GridPtr != nullptr);
                GridPtr->setTransform(InGridPtr->transform().copy());
                //TODO allow changing float-as-half under certain circumstances?
            }
            else
            {
                //TODO log error (tried to shallow copy a grid of different type)
            }
        }
        return GridPtr;
    }

    template<typename VoxelType>
    static typename openvdb::Grid<typename openvdb::tree::Tree4<VoxelType>::Type>::Ptr DeepCopyGrid(ValueTypeConstPtr& InGridPtr)
    {
        typename openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr GridPtr = nullptr;
        if (InGridPtr != nullptr)
        {
            if (InGridPtr->isType<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>())
            {
                GridPtr = InGridPtr->deepCopyGrid();
                check(GridPtr != nullptr);
                //TODO allow changing float-as-half under certain circumstances?
            }
            else
            {
                //TODO log error (tried to shallow copy a grid of different type)
            }
        }
        return GridPtr;
    }
};

#pragma warning(pop)
