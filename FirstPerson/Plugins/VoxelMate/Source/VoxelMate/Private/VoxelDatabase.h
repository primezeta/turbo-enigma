#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseProxy.h"
#include "GridCoordinateTransforms.h"
#include <vector>
#include <openvdb/tools/ValueTransformer.h>
#include <openvdb/io/Archive.h>
#include "VoxelDatabase.generated.h"

//TODO: See UWorldProxy which has checks for access from non-game threads. Might need to do the same for AVoxelDatabase

UCLASS(ClassGroup = VoxelMate, NotBlueprintable, NotPlaceable, CustomConstructor)
class AVoxelDatabase : public AActor
{
    GENERATED_BODY()

public:
	static AVoxelDatabase* VoxelMateVoxelDatabase;
	static void InitVoxelMateDatabase()
	{
		if (!VoxelMateVoxelDatabase)
		{
			VoxelMateVoxelDatabase = NewObject<AVoxelDatabase>();
			//VoxelMateVoxelDatabase->IsSafeForRootSet() TODO
			VoxelMateVoxelDatabase->AddToRoot();

			if (VoxelMateVoxelDatabase->GetNetMode() != ENetMode::NM_Client)
			{
				VoxelMateVoxelDatabase->AuthDatabaseProxy = NewObject<AVoxelDatabaseProxy>(VoxelMateVoxelDatabase);
				VoxelMateVoxelDatabase->DatabaseProxy = VoxelMateVoxelDatabase->AuthDatabaseProxy;
			}
		}
	}

    UPROPERTY()
        AVoxelDatabaseProxy* AuthDatabaseProxy;
    UPROPERTY(Transient)
        AVoxelDatabaseProxy* LocalDatabaseProxy;
    UPROPERTY(Transient, ReplicatedUsing = OnRep_SetDatabaseProxy)
        AVoxelDatabaseProxy* DatabaseProxy;
    
    UFUNCTION(Server, Reliable, WithValidation)
        void SetDatabaseProxy();
    void SetDatabaseProxy_Implementation()
    {
        OnRep_SetDatabaseProxy();
    }

    UFUNCTION()
        void OnRep_SetDatabaseProxy()
        {
            LocalDatabaseProxy = DatabaseProxy;
        }

    UFUNCTION()
        bool SetDatabaseProxy_Validate()
        {
            return true;//TODO
        }

    friend FArchive& operator<<(FArchive& Ar, AVoxelDatabase& VoxelDatabase)
    {
		VoxelDatabase.SerializeDatabaseMetadata(Ar);
		VoxelDatabase.SerializeAllGrids(Ar);
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
		Ar << *this;
    }

    virtual void BeginDestroy() override
    {
        Super::BeginDestroy();
    }

    static void Startup();
	static void Shutdown();

    const TArray<FString>& GetRegisteredGridTypeNames() const;
    const TArray<FString>& GetRegisteredMetadataTypeNames() const;
    const TArray<FString>& GetRegisteredTransformMapTypeNames() const;
	
    //TODO bool AddGridInstance

    template<typename VoxelType>
    bool AddGrid(const FText& GridDisplayText, bool SaveFloatAsHalf, FGuid& OutGridId)
    {
        bool IsGridAdded = false;
        OutGridId = FGuid::NewGuid();
        if (OutGridId.IsValid())
        {
            FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create<VoxelType>();
            GridPtr->setName(TCHAR_TO_UTF8(*OutGridId.ToString()));
            GridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
            FMetaMapFactory::InsertGridMeta<FMetadataText>(*GridPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, FMetadataText(GridDisplayText));
			AllocatedGrids.push_back(GridPtr);
            Grids.Add(OutGridId, GridPtr);
            IsGridAdded = true;
        }
        else
        {
            //TODO log warning (new grid id not valid)
            OutGridId.Invalidate(); //set all to 0
        }
        return IsGridAdded;
    }

    template<typename VoxelType>
    int32 ShallowCopyGrid(const FGuid& InGridId, const FText& GridDisplayText)
    {
        int32 GridCopyIndex = -1;
        FGridFactory::ValueTypePtr GridCopyPtr = FGridFactory::ShallowCopyGrid<VoxelType>(OriginalGridPtr);
        if (GridCopyPtr != nullptr)
        {
            FMetaMapFactory::InsertGridMeta<FMetadataText>(*GridCopyPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, FMetadataText(GridDisplayText));

            const openvdb::Name& OriginalGridName = GridPtr->getName();
            GridCopyIndex = GridCopies.Num(InGridId);

            std::ostringstream GridCopyName;
            GridCopyName << GridPtr->getName() << "_" << GridCopyIndex;
            GridPtr->setName(GridCopyName.str());

            GridCopies.AddUnique(InGridId, GridCopyPtr);
            IsGridCopied = true;
        }
        else
        {
            //TODO log error (could not copy grid; possibly different types)
        }
        return GridCopyIndex;
    }

    template<typename VoxelType>
    bool DuplicateGrid(const FGuid& InGridId, const FText& GridDisplayText, FGuid& OutDuplicateGridId)
    {
        FGridFactory::ValueTypePtr* FindGrid = Grids.Find(InGridId);
        if (!FindGrid)
        {
            //TODO log error (grid already exists - cannot overwrite it with the duplicate)
            return false;
        }

        bool IsGridDuplicated = false;
        FGridFactory::ValueTypePtr GridDuplicatePtr = FGridFactory::DeepCopyGrid<VoxelType>(*FindGrid);
        if (GridDuplicatePtr != nullptr)
        {
            OutDuplicateGridId = FGuid::NewGuid();
            check(!Grids.Contains(OutDuplicateGridId));

            //Note: save-float-as-half is automatically copied via the grid metadata
            FMetaMapFactory::InsertGridMeta<FMetadataText>(*GridDuplicatePtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, FMetadataText(GridDisplayText));
			AllocatedGrids.push_back(*GridDuplicatePtr);
            Grids.Add(OutDuplicateGridId, GridDuplicatePtr);
            IsGridDuplicated = true;
        }
        else
        {
            //TODO log error (could not copy grid; possibly different types)
        }
        return IsGridDuplicated;
    }

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
        }

        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, bool& OutIsVoxelActive) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
        }

        OutIsVoxelActive = false;
        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void GetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord, bool& OutIsActive) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
        }
        OutIsActive = false;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
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
    }

    template<typename VoxelType, typename ValueSourceType, EVoxelIterator VoxelIterType>
    VOXELMATEINLINE void SetVoxelValuesFromSource(const FGuid& GridId, const ValueSourceType &ValueSource)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<VoxelIterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> IterAdaptor;
                typedef IterAdaptor::Type IterType;
                typedef VoxelDatabaseUtil::TSetValuesOp<IterType, VoxelType, ValueSourceType> OpType;

                OpType SetValuesOp(ValueSource, TypedGridPtr->transform());
                openvdb::tools::foreach<IterType, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), SetValuesOp);
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void FillGrid(const FGuid& GridId, const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const VoxelType& InValue, const bool &InIsActive, const bool &InVoxelizeActiveTilesAfterFill)
    {
        if (FillDimensions.X == 0 || FillDimensions.Y == 0 || FillDimensions.Z == 0)
        {
            //TODO log error (fill volume is empty)
            return;
        }

        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                //const CoordBBox& bbox, const ValueType& value, bool active = true
                const openvdb::Coord FillStart(StartIndexCoord.X, StartIndexCoord.Y, StartIndexCoord.Z);
                const openvdb::Coord FillEnd(FillStart.x() + FillDimensions.X - 1, FillStart.y() + FillDimensions.Y - 1, FillStart.z() + FillDimensions.Z - 1);
                const openvdb::CoordBBox FillBBox(FillStart, FillEnd);
                TypedGridPtr->fill(FillBBox, InValue, InIsActive);

                if (InVoxelizeActiveTilesAfterFill)
                {
                    TypedGridPtr->tree().voxelizeActiveTiles();
                }
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    void ExtractGridSurface()
    {

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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FAffineCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUnitaryCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr && *FindGrid != nullptr)
        {
            VoxelDatabaseStatics::SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(**FindGrid, InCoordinateTransform);
        }
    }

    void GridRotation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
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
    friend class FVoxelMateModule;
    AVoxelDatabase();

	openvdb::MetaMap DatabaseMetadata;
	openvdb::GridPtrVec AllocatedGrids;
	TMap<FGuid, FGridFactory::ValueTypePtr> Grids;
    TMultiMap<FGuid, FGridFactory::ValueTypePtr> GridCopies;

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

	void SerializeAllGrids(FArchive& Ar)
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
			for (auto i = AllocatedGrids.begin(); i != AllocatedGrids.end(); ++i)
			{
				if ((*i) != nullptr)
				{
					GridCount++;
				}
			}

			Ar << GridCount;
			for (auto i = AllocatedGrids.begin(); i != AllocatedGrids.end(); ++i)
			{
				WriteGrid(Ar, *i);
			}
		}
	}

	VOXELMATEINLINE void ReadMetadata(FArchive& Ar)
	{
		check(Ar.IsLoading());

		TArray<ANSICHAR> DatabaseMetadataBytes;
		Ar << DatabaseMetadataBytes;

		FStreamReader DatabaseDataFileStream(DatabaseMetadataBytes, Ar.IsPersistent());
		DatabaseMetadata.readMeta(DatabaseDataFileStream.GetStream());
	}

	VOXELMATEINLINE void WriteMetadata(FArchive& Ar)
	{
		check(Ar.IsSaving());
		
		TArray<ANSICHAR> DatabaseMetadataBytes;
		FStreamWriter DatabaseDataFileStream(DatabaseMetadataBytes, Ar.IsPersistent());
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

			FStreamReader GridReader(GridBytes, Ar.IsPersistent());
			GridPtr->readMeta(GridReader.GetStream());
			GridPtr->readTopology(GridReader.GetStream());
			GridPtr->readTransform(GridReader.GetStream());
			GridPtr->readBuffers(GridReader.GetStream());

			FGuid GridId;
			FGuid::Parse(UTF8_TO_TCHAR(GridPtr->getName().c_str()), GridId);
			Grids.Add(GridId, GridPtr);
			AllocatedGrids.push_back(GridPtr);
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
			FStreamWriter GridWriter(GridBytes, Ar.IsPersistent());
			GridPtr->writeMeta(GridWriter.GetStream());
			GridPtr->writeTopology(GridWriter.GetStream());
			GridPtr->writeTransform(GridWriter.GetStream());
			GridPtr->writeBuffers(GridWriter.GetStream());
			Ar << GridBytes;
		}
	}
};
