#pragma once
#include "VoxelDatabaseTypeFactory.h"
#include "VoxelDatabaseTypes.h"
#include "openvdb/openvdb.h"

typedef openvdb::Grid<openvdb::tree::Tree4<FVoxel>::Type> GridType;

struct FGridFactory : public FVoxelDatabaseTypeFactory<openvdb::GridBase>
{
    static TMap<FString, int32> CachedTopologySizeByTreeType;
    uint8 CompressionFlags;

    template<typename VoxelType>
    static void Register()
    {
        try
        {
            GridType::registerGrid();
            RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<VoxelType>()), false));
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (grid type already registered)
        }
    }

    template<typename VoxelType>
    static bool IsRegistered()
    {
        return GridType::isRegistered();
    }

    template<typename VoxelType>
    static ValueTypePtr Create()
    {
        ValueTypePtr GridPtr = nullptr;
        try
        {
            GridPtr = GridType::create();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (grid type name is not registered)
        }
        check(GridPtr != nullptr);
        return GridPtr;
    }

	static ValueTypePtr CreateType(const FString& TypeName)
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

    template<typename VoxelType>
    static typename openvdb::Grid<typename openvdb::tree::Tree4<VoxelType>::Type>::Ptr ShallowCopyGrid(const openvdb::GridBase::ConstPtr& InGridPtr)
    {
        typename GridType::Ptr GridPtr = nullptr;
        if (InGridPtr != nullptr)
        {
            if (InGridPtr->isType<GridType>())
            {
                GridPtr = GridType::Ptr(new GridType(*InGridPtr, openvdb::ShallowCopy()));
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
};
