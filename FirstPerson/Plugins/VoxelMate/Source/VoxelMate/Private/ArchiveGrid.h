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

    template<typename ValueType>
    static void RegisterGridType()
    {
        //if (!openvdb::Grid<openvdb::tree::Tree3<ValueType>::Type>::isRegistered())
        //{
        //    openvdb::Grid<openvdb::tree::Tree3<ValueType>::Type>::registerGrid();
        //    FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(openvdb::Grid<openvdb::tree::Tree3<ValueType>::Type>::gridType().c_str()));
        //}

        if (!openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::isRegistered())
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::registerGrid();
            FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::gridType().c_str()));
        }

        //if (!openvdb::Grid<openvdb::tree::Tree5<ValueType>::Type>::isRegistered())
        //{
        //    openvdb::Grid<openvdb::tree::Tree5<ValueType>::Type>::registerGrid();
        //    FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(openvdb::Grid<openvdb::tree::Tree5<ValueType>::Type>::gridType().c_str()));
        //}
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::GridBase::isRegistered(TCHAR_TO_UTF8(*TypeName));
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        ValueTypePtr GridPtr = nullptr;
        try
        {
            GridPtr = openvdb::GridBase::createGrid(TCHAR_TO_UTF8(*TypeName));
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (grid type name is not registered)
        }
        return GridPtr;
    }

    template<typename GridType>
    static ValueTypePtr Create()
    {
        ValueTypePtr GridPtr = nullptr;
        try
        {
            GridPtr = openvdb::createGrid<GridType>();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (grid type name is not registered)
        }
        return GridPtr;
    }

    template<typename GridType>
    static typename GridType::Ptr ShallowCopyGrid(ValueTypeConstPtr& InGridPtr)
    {
        typename GridType::Ptr GridPtr = nullptr;
        if (InGridPtr != nullptr)
        {
            if (InGridPtr->isType<GridType>())
            {
                GridPtr = GridType(*InGridPtr, openvdb::ShallowCopy);
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

    template<typename GridType>
    static typename GridType::Ptr DeepCopyGrid(ValueTypeConstPtr& InGridPtr)
    {
        typename GridType::Ptr GridPtr = nullptr;
        if (InGridPtr != nullptr)
        {
            if (InGridPtr->isType<GridType>())
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

    static void ClearRegistry()
    {
        openvdb::GridBase::clearRegistry();
    }
};

#pragma warning(pop)
