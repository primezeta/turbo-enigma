#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveMetaValue.h"
#include "ArchiveTransformMap.h"
#include "VoxelDatabaseProxy.h"
#include "GridCoordinateTransforms.h"
#include "VoxelValueSources.h"
#include "EngineGridTypes.h"
#include <vector>
#include <openvdb/tools/ValueTransformer.h>
#include <openvdb/io/Archive.h>
#include "VoxelDatabase.generated.h"

//TODO: See UWorldProxy which has checks for access from non-game threads. Might need to do the same for AVoxelDatabase
struct FGridBaseValuesGeneratorArray;

USTRUCT()
struct FGridBaseValuesGeneratorItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGridBaseValuesGeneratorItem()
	{}

	FGridBaseValuesGeneratorItem(const FText& GridText, UValueSource* Source)
		: DisplayText(GridText), ValueSource(Source)
	{
		GridId = FGuid::NewGuid();
	}

	friend FArchive& operator<<(FArchive& Ar, FGridBaseValuesGeneratorItem& Item)
	{
		Item.Serialize(Ar);
		return Ar;
	}

	UPROPERTY()
		FGuid GridId;
	UPROPERTY()
		FText DisplayText;
	UPROPERTY()
		UValueSource* ValueSource;

	void Serialize(FArchive& Ar)
	{
		Ar << GridId;
		Ar << DisplayText;
		ValueSource->Serialize(Ar);
	}

	void PreReplicatedRemove(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}

	void PostReplicatedChange(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}

	void PostReplicatedAdd(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}
};

USTRUCT()
struct FGridBaseValuesGeneratorArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FGridBaseValuesGeneratorItem> Items;

	void Serialize(FArchive& Ar)
	{
		Items.BulkSerialize(Ar);
		MarkArrayDirty();
	}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGridBaseValuesGeneratorItem, FGridBaseValuesGeneratorArray>(Items, DeltaParms, *this);
	}

	const FGridBaseValuesGeneratorItem& AddItem(FGridBaseValuesGeneratorItem&& Item)
	{
		const int32 Index = Items.Add(Item);
		MarkItemDirty(Items[Index]);
		return Items[Index];
	}

	void RemoveItem(const FGuid& GridId)
	{
		for (auto i = Items.CreateIterator(); i; ++i)
		{
			if (i->GridId == GridId)
			{
				Items.RemoveAt(i.GetIndex());
				MarkArrayDirty();
				return;
			}
		}
	}
};

template<>
struct TStructOpsTypeTraits<FGridBaseValuesGeneratorArray> : public TStructOpsTypeTraitsBase
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

UCLASS(ClassGroup = VoxelMate, NotBlueprintable, NotPlaceable, CustomConstructor)
class AVoxelDatabase : public AActor
{
    GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing=OnRep_GridBaseValuesGeneratorArray)
		FGridBaseValuesGeneratorArray GridBaseValuesGenerators;

	UPROPERTY()
		FString DatabasePath;

	UFUNCTION()
		void AddGrid(UValueSource* ValueSource, const FText& GridText, FGuid& OutGridId)
		{
			const ENetMode NetMode = GetNetMode();
			if (NetMode != ENetMode::NM_Client)
			{
				OutGridId = GridBaseValuesGenerators.AddItem(FGridBaseValuesGeneratorItem(GridText, ValueSource)).GridId;
			}
			else
			{
				OutGridId.Invalidate();
			}
		}

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void ChangeVoxel(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive);
	
	bool ChangeVoxel_Validate(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive)
	{
		return true; //TODO
	}

	void ChangeVoxel_Implementation(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive)
	{
		const openvdb::GridBase::Ptr* FindServerGrid = nullptr;
		const openvdb::GridBase::Ptr* FindClientGrid = nullptr;
		const ENetMode NetMode = GetNetMode();

		if (NetMode == ENetMode::NM_DedicatedServer)
		{
			FindServerGrid = GridChanges.Find(GridId);
		}
		else if (NetMode == ENetMode::NM_Client)
		{
			FindClientGrid = Grids.Find(GridId);
		}
		else
		{
			FindServerGrid = GridChanges.Find(GridId);
			FindClientGrid = Grids.Find(GridId);
		}

		if (FindServerGrid)
		{
			ChangeVoxelValue(*FindServerGrid, IndexCoord, Voxel, IsActive);
		}

		if (FindClientGrid)
		{
			ChangeVoxelValue(*FindClientGrid, IndexCoord, Voxel, IsActive);
		}
	}

	VOXELMATEINLINE void ChangeVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& IndexCoord, const FVoxelBase& Voxel, const bool& IsActive)
	{
		check(GridPtr != nullptr);
		switch (Voxel.VoxelType)
		{
		case(EVoxelType::Bool):
			SetVoxelValue<FVoxelBool>(GridPtr, IndexCoord, static_cast<const FVoxelBool&>(Voxel), IsActive);
			break;
		case(EVoxelType::UInt8):
			SetVoxelValue<FVoxelUInt8>(GridPtr, IndexCoord, static_cast<const FVoxelUInt8&>(Voxel), IsActive);
			break;
		case(EVoxelType::Int32):
			SetVoxelValue<FVoxelInt32>(GridPtr, IndexCoord, static_cast<const FVoxelInt32&>(Voxel), IsActive);
			break;
		case(EVoxelType::Float):
			SetVoxelValue<FVoxelFloat>(GridPtr, IndexCoord, static_cast<const FVoxelFloat&>(Voxel), IsActive);
			break;
		case(EVoxelType::IntVector):
			SetVoxelValue<FVoxelIntVector>(GridPtr, IndexCoord, static_cast<const FVoxelIntVector&>(Voxel), IsActive);
			break;
		case(EVoxelType::Vector):
			SetVoxelValue<FVoxelVector>(GridPtr, IndexCoord, static_cast<const FVoxelVector&>(Voxel), IsActive);
			break;
		default:
			check(false);
		}
	}

	UFUNCTION()
		void OnRep_GridBaseValuesGeneratorArray()
		{
			check(GetNetMode() != ENetMode::NM_DedicatedServer);

			for (auto i = GridBaseValuesGenerators.ItemMap.CreateIterator(); i; ++i)
			{
				if (i.Key() != INDEX_NONE)
				{
					const int32 ItemIndex = i.Value();
					const FGridBaseValuesGeneratorItem& Item = GridBaseValuesGenerators.Items[ItemIndex];

					openvdb::GridBase::Ptr GridPtr = nullptr;
					switch (Item.ValueSource->VoxelType)
					{
					case EVoxelType::Bool:
						CreateReplicatedGrid<FVoxelBool>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelBool, UVoxelBoolSource>(GridPtr, *Cast<UVoxelBoolSource>(Item.ValueSource));
						break;
					case EVoxelType::UInt8:
						CreateReplicatedGrid<FVoxelUInt8>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelUInt8, UVoxelUInt8Source>(GridPtr, *Cast<UVoxelUInt8Source>(Item.ValueSource));
						break;
					case EVoxelType::Int32:
						CreateReplicatedGrid<FVoxelInt32>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelInt32, UVoxelInt32Source>(GridPtr, *Cast<UVoxelInt32Source>(Item.ValueSource));
						break;
					case EVoxelType::Float:
						CreateReplicatedGrid<FVoxelFloat>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelFloat, UVoxelFloatSource>(GridPtr, *Cast<UVoxelFloatSource>(Item.ValueSource));
						break;
					case EVoxelType::IntVector:
						CreateReplicatedGrid<FVoxelIntVector>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelIntVector, UVoxelIntVectorSource>(GridPtr, *Cast<UVoxelIntVectorSource>(Item.ValueSource));
						break;
					case EVoxelType::Vector:
						CreateReplicatedGrid<FVoxelVector>(Item.GridId, Item.DisplayText, false, GridPtr);
						CreateVoxelValuesFromSource<FVoxelVector, UVoxelVectorSource>(GridPtr, *Cast<UVoxelVectorSource>(Item.ValueSource));
						break;
					default:
						check(false);
					}
				}
			}
		}

    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
		if (!IsDefaultSubobject() && GetNetMode() == ENetMode::NM_DedicatedServer)
		{
			GridBaseValuesGenerators.Serialize(Ar);
		}
		//FArchive* Reader = IFileManager::Get().CreateFileReader(*DatabaseDirectory);
		//if (Reader)
		//{
		//	VoxelDatabase->Serialize(*Reader);
		//}
		//FArchive* Writer = IFileManager::Get().CreateFileWriter(*DatabaseDirectory);
		//if (Writer)
		//{
		//	VoxelDatabase->Serialize(*Writer);
		//}
    }

    virtual void BeginDestroy() override
    {
        Super::BeginDestroy();
    }

	static void Startup()
	{
		//TODO: Note that the following BLOSC related stuff is in openvdb::initialize
		//#ifdef OPENVDB_USE_BLOSC
		//    blosc_init();
		//    if (blosc_set_compressor("lz4") < 0) {
		//        OPENVDB_LOG_WARN("Blosc LZ4 compressor is unavailable");
		//    }
		//    /// @todo blosc_set_nthreads(int nthreads);
		//#endif
		FGridFactory::RegisterSupportedTypes();
		FMetaValueFactory::RegisterSupportedTypes();
		FTransformMapFactory::RegisterSupportedTypes();
	}

	static void Shutdown()
	{
		FGridFactory::UnregisterSupportedTypes();
		FMetaValueFactory::UnregisterSupportedTypes();
		FTransformMapFactory::UnregisterSupportedTypes();
	}

	const TArray<FString>& GetRegisteredGridTypeNames() const
	{
		return FGridFactory::GetRegisteredTypeNames();
	}

	const TArray<FString>& GetRegisteredMetadataTypeNames() const
	{
		return FMetaValueFactory::GetRegisteredTypeNames();
	}

	const TArray<FString>& GetRegisteredTransformMapTypeNames() const
	{
		return FTransformMapFactory::GetRegisteredTypeNames();
	}
	
    //TODO bool AddGridInstance

	template<typename VoxelType>
	bool CreateReplicatedGrid(const FGuid& GridId, const FText& GridDisplayText, bool SaveFloatAsHalf, openvdb::GridBase::Ptr& OutGridPtr)
	{
		check(GetNetMode() != ENetMode::NM_DedicatedServer);

		bool IsGridAdded = false;
		if (GridId.IsValid() && !Grids.Contains(GridId))
		{
			OutGridPtr = FGridFactory::Create<VoxelType>();
			OutGridPtr->setName(TCHAR_TO_UTF8(*GridId.ToString()));
			OutGridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
			FMetaMapFactory::InsertGridMeta<FMetadataText>(*OutGridPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, FMetadataText(GridDisplayText));
			Grids.Add(GridId, OutGridPtr);

			GridChanges.Add(GridId, FGridFactory::ShallowCopyGrid<VoxelType>(OutGridPtr));
			IsGridAdded = true;
		}
		else
		{
			//TODO log warning (new grid id not valid)
		}
		return IsGridAdded;
	}

	template<typename VoxelType, typename ValueSourceType>
	void CreateVoxelValuesFromSource(const openvdb::GridBase::Ptr& GridPtr, UValueSource& ValueSource)
	{
		check(GetNetMode() != ENetMode::NM_DedicatedServer);

		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
		if (TypedGridPtr)
		{
			const openvdb::Coord FillStart(0, 0, 0);
			const openvdb::Coord FillEnd(ValueSource.VolumeSize.X - 1, ValueSource.VolumeSize.Y - 1, ValueSource.VolumeSize.Z - 1);
			const openvdb::CoordBBox FillBBox(FillStart, FillEnd);
			openvdb::TypedMetadata<VoxelType>::Ptr BackgroundValueMetadata = boost::static_pointer_cast<openvdb::TypedMetadata<VoxelType>>(TypedGridPtr->tree().getBackgroundValue());
			TypedGridPtr->fill(FillBBox, BackgroundValueMetadata->value(), true);
			TypedGridPtr->tree().voxelizeActiveTiles();

			if (ValueSource.CoordTransformType == ECoordinateTransformType::Affine)
			{
				FAffineCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FAffineCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FAffineCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::Unitary)
			{
				FUnitaryCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FUnitaryCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FUnitaryCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::Scale)
			{
				FScaleCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FScaleCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::UniformScale)
			{
				FUniformScaleCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FUniformScaleCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::Translation)
			{
				FTranslationCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FTranslationCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::ScaleTranslation)
			{
				FScaleTranslationCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FScaleTranslationCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::UniformScaleTranslation)
			{
				FUniformScaleCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FUniformScaleCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else if (ValueSource.CoordTransformType == ECoordinateTransformType::NonlinearFrustum)
			{
				FTranslationCoordinateTransform CoordTransform;
				ValueSource.ReadCoordTransform<FTranslationCoordinateTransform>(CoordTransform);
				VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(*GridPtr, CoordTransform);
			}
			else
			{
				check(false);
			}

			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<EVoxelIterator::ActiveVoxelsIter, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> IterAdaptor;
			typedef IterAdaptor::Type IterType;
			typedef VoxelDatabaseUtil::TSetValuesOp<IterType, VoxelType, ValueSourceType> OpType;

			OpType SetValuesOp(static_cast<const ValueSourceType&>(ValueSource), TypedGridPtr->transform());
			openvdb::tools::foreach<IterType, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), SetValuesOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord) const
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
            return Accessor.getValue(Coord);
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }

        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord, bool& OutIsVoxelActive) const
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
            OutIsVoxelActive = Accessor.isValueOn(Coord);
            return Accessor.getValue(Coord);
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }

        OutIsVoxelActive = false;
        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void GetVoxelIsActive(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord, bool& OutIsActive) const
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
            OutIsActive = Accessor.isValueOn(Coord);
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }

        OutIsActive = false;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue)
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
            Accessor.setValueOnly(Coord, InVoxelValue);
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue, const bool& InIsActive)
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);

            if (InIsActive)
            {
                Accessor.setValueOn(Coord, InVoxelValue);
            }
            else
            {
                Accessor.setValueOff(Coord, InVoxelValue);
            }
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const bool& InIsActive)
    {
        const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
        if (TypedGridPtr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
            const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
            Accessor.setActiveState(Coord, InIsActive);
        }
        else
        {
            //TODO log error (grid types mismatched)
            check(false); //TODO handle error
        }
    }

	template<typename VoxelType, EVoxelIterator VoxelIterType>
    void ExtractGridSurface(const openvdb::GridBase::Ptr& GridPtr)
    {
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
		if (TypedGridPtr)
		{
			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<VoxelIterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> IterAdaptor;
			typedef IterAdaptor::Type IterType;
			typedef VoxelDatabaseUtil::TExtractSurfaceOp<IterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> OpType;

			OpType ExtractSurfaceOp(*TypedGridPtr);
			openvdb::tools::foreach<IterType, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), ExtractSurfaceOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
    }

	template<typename VoxelType, EVoxelIterator VoxelIterType>
	void MeshGridSurface(const openvdb::GridBase::Ptr& GridPtr)
	{
		const openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(GridPtr);
		if (TypedGridPtr)
		{
			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<VoxelIterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> IterAdaptor;
			typedef IterAdaptor::Type IterType;
			typedef VoxelDatabaseUtil::TExtractSurfaceOp<IterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> OpType;

			OpType ExtractSurfaceOp(*TypedGridPtr);
			openvdb::tools::foreach<IterType, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), ExtractSurfaceOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

    template<typename MetadataType>
    bool AddMetadata(FGuid& OutMetadataId)
    {
        bool IsMetadataAdded = false;
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create<MetadataType>();
        if (MetaPtr != nullptr)
        {
            OutMetadataId = FGuid::NewGuid();
            check(!DatabaseMetadata.Contains(OutMetadataId));
			DatabaseMetadata.insertMeta(OutMetadataId.ToString(), *MetaPtr);
            IsMetadataAdded = true;
        }
        return IsMetadataAdded;
    }

    void SetCoordinateTransform(const FGuid& GridId, const FAffineCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FAffineCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUnitaryCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void GridRotation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Axis RotationAxis = VoxelDatabaseStatics::GetAxisType(Axis);
            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preRotate(Radians, RotationAxis);
            }
            else
            {
                (*FindGrid)->transform().postRotate(Radians, RotationAxis);
            }
        }
    }

    void GridTranslation(const FGuid& GridId, ETransformOp Op, const FVector &InTranslation)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Translation(InTranslation.X, InTranslation.Y, InTranslation.Z);

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preTranslate(Translation);
            }
            else
            {
                (*FindGrid)->transform().postTranslate(Translation);
            }
        }
    }

    void GridScale(const FGuid& GridId, ETransformOp Op, const FVector &InScale)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Scale(InScale.X, InScale.Y, InScale.Z);

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preScale(Scale);
            }
            else
            {
                (*FindGrid)->transform().postScale(Scale);
            }
        }
    }

    void GridUniformScale(const FGuid& GridId, ETransformOp Op, float Scale)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preScale(Scale);
            }
            else
            {
                (*FindGrid)->transform().postScale(Scale);
            }            
        }
    }

    void GridShear(const FGuid& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Axis ShearAxis0 = VoxelDatabaseStatics::GetAxisType(Axis0);
            const openvdb::math::Axis ShearAxis1 = VoxelDatabaseStatics::GetAxisType(Axis1);

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preShear(Shear, ShearAxis0, ShearAxis1);
            }
            else
            {
                (*FindGrid)->transform().postShear(Shear, ShearAxis0, ShearAxis1);
            }
        }
    }

    void GridMatrix4dMultiply(const FGuid& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Mat4d Matrix(
                InX.X, InX.Y, InX.Z, InX.W,
                InY.X, InY.Y, InY.Z, InY.W,
                InZ.X, InZ.Y, InZ.Z, InZ.W,
                InW.X, InW.Y, InW.Z, InW.W
            );

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preMult(Matrix);
            }
            else
            {
                (*FindGrid)->transform().postMult(Matrix);
            }            
        }
    }

    void GridMatrix3dMultiply(const FGuid& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ)
    {
        const openvdb::GridBase::Ptr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Mat3d Matrix(
                InX.X, InX.Y, InX.Z,
                InY.X, InY.Y, InY.Z,
                InZ.X, InZ.Y, InZ.Z
            );

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preMult(Matrix);
            }
            else
            {
                (*FindGrid)->transform().postMult(Matrix);
            }
        }
    }

private:
	openvdb::MetaMap DatabaseMetadata;
	TMap<FGuid, openvdb::GridBase::Ptr> Grids;
	TMap<FGuid, openvdb::GridBase::Ptr> GridChanges;

	friend class FVoxelMateModule;
	AVoxelDatabase()
	{
		AddToRoot();
		SetReplicates(true);
		SetReplicateMovement(false);
		SetActorEnableCollision(false);
		bNetTemporary = 0;
		bAutoDestroyWhenFinished = 0;
		bCanBeDamaged = 0;
	}

	void SerializeDatabaseMetadata(FArchive& Ar)
	{
		if (Ar.IsLoading())
		{
			ReadMetadata(Ar);
		}
		else
		{
			WriteMetadata(Ar);
		}
	}

	void SerializeGridChanges(FArchive& Ar)
	{
		int32 GridCount = -1;
		if (Ar.IsLoading())
		{
			Ar << GridCount;
			for (int32 i = 0; i < GridCount; ++i)
			{
				ReadGrid(Ar);
			}
		}
		else
		{
			//First get the non-null grid count
			for (auto i = GridChanges.CreateConstIterator(); i; ++i)
			{
				if (i.Value() != nullptr)
				{
					GridCount++;
				}
			}

			Ar << GridCount;
			for (auto i = GridChanges.CreateConstIterator(); i; ++i)
			{
				WriteGrid(Ar, i.Value());
			}
		}
	}

	VOXELMATEINLINE void ReadMetadata(FArchive& Ar)
	{
		check(Ar.IsLoading());

		TArray<ANSICHAR> DatabaseMetadataBytes;
		Ar << DatabaseMetadataBytes;

		FStreamReader<ANSICHAR> DatabaseDataFileStream(DatabaseMetadataBytes, Ar.IsPersistent());
		DatabaseMetadata.readMeta(DatabaseDataFileStream.GetStream());
	}

	VOXELMATEINLINE void WriteMetadata(FArchive& Ar)
	{
		check(Ar.IsSaving());
		
		TArray<ANSICHAR> DatabaseMetadataBytes;
		FStreamWriter<ANSICHAR> DatabaseDataFileStream(DatabaseMetadataBytes, Ar.IsPersistent());
		DatabaseMetadata.writeMeta(DatabaseDataFileStream.GetStream());

		Ar << DatabaseMetadataBytes;
	}

	VOXELMATEINLINE void ReadGrid(FArchive& Ar)
	{
		check(Ar.IsLoading());

		FString GridName;
		Ar << GridName;

		FString GridTypeName;
		Ar << GridTypeName;

		openvdb::GridBase::Ptr GridPtr = FGridFactory::CreateType(GridTypeName);
		if (GridPtr != nullptr)
		{
			TArray<ANSICHAR> GridBytes;
			Ar << GridBytes;

			FStreamReader<ANSICHAR> GridReader(GridBytes, Ar.IsPersistent());
			GridPtr->readMeta(GridReader.GetStream());
			GridPtr->readTopology(GridReader.GetStream());
			GridPtr->readTransform(GridReader.GetStream());
			GridPtr->readBuffers(GridReader.GetStream());

			FGuid GridId;
			FGuid::Parse(UTF8_TO_TCHAR(GridPtr->getName().c_str()), GridId);
			GridChanges.Add(GridId, GridPtr);
		}
		else
		{
			//Skip past the array portion
			int32 GridByteCount = 0;
			Ar << GridByteCount;

			const int64 CurrentPos = Ar.Tell();
			Ar.Seek(CurrentPos + (int64)GridByteCount);
		}
	}

	VOXELMATEINLINE void WriteGrid(FArchive& Ar, const openvdb::GridBase::Ptr& GridPtr)
	{
		check(Ar.IsSaving());

		if (GridPtr != nullptr)
		{
			FString GridName = UTF8_TO_TCHAR(GridPtr->getName().c_str());
			Ar << GridName;

			FString GridTypeName = UTF8_TO_TCHAR(GridPtr->type().c_str());
			Ar << GridTypeName;

			TArray<ANSICHAR> GridBytes;
			FStreamWriter<ANSICHAR> GridWriter(GridBytes, Ar.IsPersistent());
			GridPtr->writeMeta(GridWriter.GetStream());
			GridPtr->writeTopology(GridWriter.GetStream());
			GridPtr->writeTransform(GridWriter.GetStream());
			GridPtr->writeBuffers(GridWriter.GetStream());
			Ar << GridBytes;
		}
	}
};
