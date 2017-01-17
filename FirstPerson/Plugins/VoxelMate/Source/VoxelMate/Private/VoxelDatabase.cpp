#include "VoxelMatePCH.h"
#include "VoxelDatabase.h"
#include "VoxelDatabaseStatics.h"

struct FGridRegionCollection
{
	FGridRegionCollection()
	{}

	FGridRegionCollection(UGridSource* Source)
		: GridSource(Source)
	{}

	UGridSource* GridSource;
	TMap<FIntVector, openvdb::GridBase::Ptr> GridRegions;
};

//Grid data for the server side contains changed voxels.
//Grid data for the client side contains all the grid data with grid changes applied to it.
static TMap<FGuid, FGridRegionCollection> GridData;

//TODO move this function to grid statics or make as a member
static openvdb::GridBase::Ptr CreateGridFromSource(UGridSource* GridSource)
{
	//check(GridSource);
	openvdb::GridBase::Ptr GridPtr = GridType::create();

	//const FIntVector& RegionSize = GridSource->RegionVoxelCount;
	//const openvdb::Vec3d RegionSizeMetaValue((float)RegionSize.X, (float)RegionSize.Y, (float)RegionSize.Z);

	////Save the region size as metadata in case the grid source is later being created as a result of reading the grid from file?
	//openvdb::TypedMetadata<FIntVector> RegionVoxelCountMetaValue(GridSource->RegionVoxelCount);
	//GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::MetaNameGridRegionScale), RegionVoxelCountMetaValue);
	//const openvdb::Coord startIndexCoord = openvdb::Coord((int32)regionIndex.X, (int32)regionIndex.Y, (int32)regionIndex.Z);
	//const openvdb::Coord endIndexCoord = openvdb::Coord((int32)regionIndex.X + 1, (int32)regionIndex.Y + 1, (int32)regionIndex.Z + 1);
	//const openvdb::Vec3d regionStart = regionSizeMetaValue.applyMap(openvdb::Vec3d((double)startIndexCoord.x(), (double)startIndexCoord.y(), (double)startIndexCoord.z()));
	//const openvdb::Vec3d regionEnd = regionSizeMetaValue.applyMap(openvdb::Vec3d((double)endIndexCoord.x(), (double)endIndexCoord.y(), (double)endIndexCoord.z()));
	//const openvdb::Vec3d regionDimensions = regionEnd - regionStart;

	//const FVector startWorld(regionStart.x(), regionStart.y(), regionStart.z());
	//const FVector endWorld(regionEnd.x() - 1, regionEnd.y() - 1, regionEnd.z() - 1); //Minus 1 of each coordinate just for the display string. The value is not used
	//const FIntVector dimensions = FIntVector(regionDimensions.x(), regionDimensions.y(), regionDimensions.z()) - FIntVector(1, 1, 1);
	//gridID = gridName + TEXT("[") + startWorld.ToString() + TEXT("]");
	//VdbHandlePrivatePtr->AddGrid(gridID, dimensions, startWorld, voxelSize, sectionBuffers);
	return GridPtr;
}

void AVoxelDatabase::InitializeAuthGridSource(UGridSource* GridSourceTemplate, TSubclassOf<UGridSource> GridSourceType)
{
	if (HasAuthority() && GridSourceTemplate && !GridSource && GridSourceTemplate->GetClass()->IsChildOf(GridSourceType))
	{
		GridSource = NewObject<UGridSource>(this, GridSourceType, NAME_None, EObjectFlags::RF_NoFlags, GridSourceTemplate);
		if (GridSource)
		{
			check(!GridData.Contains(GridSource->GridId));
			GridData.Add(GridSource->GridId, FGridRegionCollection(GridSource));
		}
	}
}

void AVoxelDatabase::OnRep_GridSource()
{
	if (IsNetMode(ENetMode::NM_DedicatedServer))
	{
		return;
	}

	GridData.FindOrAdd(GridSource->GridId);
}

void AVoxelDatabase::GenerateGridValues(const FGuid& GridId, const FVector& Location)
{
	if (IsNetMode(ENetMode::NM_DedicatedServer))
	{
		return;
	}

	FGridRegionCollection* FindGridRegionCollection = GridData.Find(GridId);
	if (!FindGridRegionCollection)
	{
		check(false);
		return;
	}

	FGridRegionCollection& GridRegionCollection = *FindGridRegionCollection;
	check(GridRegionCollection.GridSource);

	const FIntVector& RegionVoxelCount = GridRegionCollection.GridSource->RegionVoxelCount;
	if (RegionVoxelCount.X < 1 || RegionVoxelCount.Y < 1 || RegionVoxelCount.Z < 1)
	{
		check(false);
		return;
	}

	const FIntVector& NumActiveAdjacentRegions = GridRegionCollection.GridSource->NumActiveAdjacentRegions;
	if (NumActiveAdjacentRegions.X < 0 || NumActiveAdjacentRegions.Y < 0 || NumActiveAdjacentRegions.Z < 0)
	{
		check(false);
		return;
	}

	const openvdb::math::ScaleMap RegionSizeMap(openvdb::Vec3d((float)RegionVoxelCount.X, (float)RegionVoxelCount.Y, (float)RegionVoxelCount.Z));
	const openvdb::Vec3d RegionSizeInverse = RegionSizeMap.applyInverseMap(openvdb::Vec3d((float)Location.X, (float)Location.Y, (float)Location.Z));
	const FIntVector RegionIndexStart = FIntVector((int32)RegionSizeInverse.x(), (int32)RegionSizeInverse.y(), (int32)RegionSizeInverse.z());

	//Generate grid data for each adjacent region
	for (int32 x = -NumActiveAdjacentRegions.X; x <= NumActiveAdjacentRegions.X; ++x)
	{
		for (int32 y = -NumActiveAdjacentRegions.Y; y <= NumActiveAdjacentRegions.Y; ++y)
		{
			for (int32 z = -NumActiveAdjacentRegions.Z; z <= NumActiveAdjacentRegions.Z; ++z)
			{
				//Assuming voxels are 1:1
				const FIntVector RegionIndex(RegionIndexStart.X + x, RegionIndexStart.Y + y, RegionIndexStart.Z + z);
				openvdb::GridBase::Ptr& GridPtr = GridRegionCollection.GridRegions.FindOrAdd(RegionIndex);
				if (GridPtr == nullptr)
				{
					const openvdb::Vec3d RegionLocation = RegionSizeMap.applyMap(openvdb::Vec3d((float)RegionIndex.X, (float)RegionIndex.Y, (float)RegionIndex.Z));
					const FVector RegionTranslation = FVector((float)RegionLocation.x(), (float)RegionLocation.y(), (float)RegionLocation.z());
					VoxelDatabaseStatics::CreateGridData(*GridRegionCollection.GridSource, RegionTranslation, false /*TODO save float as half?*/, GridPtr);
				}
				check(GridPtr != nullptr);
			}
		}
	}
}

void AVoxelDatabase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelDatabase, GridSource);
}

bool AVoxelDatabase::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool SubobjectWasChanged = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (GridSource != nullptr)
	{
		SubobjectWasChanged |= Channel->ReplicateSubobject(GridSource, *Bunch, *RepFlags);
	}
	return SubobjectWasChanged;
}

void AVoxelDatabase::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	//if (!IsDefaultSubobject() && GetNetMode() == ENetMode::NM_DedicatedServer)
	//{
	//	GridBaseValuesGenerators.Serialize(Ar);
	//}
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

//void AVoxelDatabase::ServerInitializeDatabase(const FString& Filename)
//{
	//const ENetMode NetMode = GetNetMode();
	//if (NetMode != ENetMode::NM_DedicatedServer)
	//{
	//	return;
	//}

	//if (!ServerDatabaseIsInitialized)
	//{
	//	TUniquePtr<FArchive> Reader = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*Filename, 0));
	//	VoxelDatabaseStatics::SerializeGridData(*Reader, GridData);
	//	SourceFilename = Filename;
	//	ServerDatabaseIsInitialized = true;

	//	for (auto i = GridData.CreateConstIterator(); i; ++i)
	//	{
	//		VoxelDatabaseStatics::ServerUpdateChangedVoxelValues(i->Value, *this);
	//	}
	//}
//}

//void AVoxelDatabase::OnRep_ServerDatabaseIsInitialized()
//{
//	const ENetMode NetMode = GetNetMode();
//	if (NetMode == ENetMode::NM_DedicatedServer)
//	{
//		return;
//	}
//	
//}

//bool AVoxelDatabase::ChangeVoxel_Validate(const FIntVector& GridId, const FIntVector& IndexCoord, const FVoxel& Voxel, bool IsActive)
//{
//	NET_LOG(this, LogVoxelMate, "%s", *GridId.ToString());
//
//	return true; //TODO
//}
//
//void AVoxelDatabase::ChangeVoxel_Implementation(const FIntVector& GridId, const FIntVector& IndexCoord, const FVoxel& Voxel, bool IsActive)
//{
//	NET_LOG(this, LogVoxelMate, "%s.%s", *GridId.ToString(), *IndexCoord.ToString());
//
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid)
//	{
//		VoxelDatabaseStatics::ChangeVoxelValue(*FindGrid, IndexCoord, Voxel, IsActive);
//	}
//}
//
//void AVoxelDatabase::OnRep_GridBaseValuesGeneratorArray()
//{
//	NET_LOG(this, LogVoxelMate, "");
//
//	if (GetNetMode() == ENetMode::NM_DedicatedServer)
//	{
//		return;
//	}
//
//	for (auto i = GridBaseValuesGenerators.ItemMap.CreateIterator(); i; ++i)
//	{
//		if (i.Key() != INDEX_NONE)
//		{
//			const int32 ItemIndex = i.Value();
//			const FGridBaseValuesGeneratorItem& Item = GridBaseValuesGenerators.Items[ItemIndex];
//
//			openvdb::GridBase::Ptr GridPtr = nullptr;
//			VoxelDatabaseStatics::CreateGridData(Item.GridId, Item.DisplayText, false, GridPtr);
//			VoxelDatabaseStatics::CreateVoxelValuesFromSource(GridPtr, *Item.GridSource);
//			GridData.Add(Item.GridId, GridPtr);
//		}
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformAffine_Implementation(const FIntVector& GridId, const FAffineCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FAffineCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformUnitary_Implementation(const FIntVector& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FUnitaryCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformScale_Implementation(const FIntVector& GridId, const FScaleCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformUniformScale_Implementation(const FIntVector& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformTranslation_Implementation(const FIntVector& GridId, const FTranslationCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformScaleTranslation_Implementation(const FIntVector& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformUniformScaleTranslation_Implementation(const FIntVector& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::SetCoordinateTransformNonlinearFrustum_Implementation(const FIntVector& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr && *FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(**FindGrid, InCoordinateTransform);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateRotation_Implementation(const FIntVector& GridId, ETransformOp Op, float Radians, EAxis::Type Axis)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridRotation(**FindGrid, Op, Radians, Axis);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateTranslation_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector &InTranslation)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridTranslation(**FindGrid, Op, InTranslation);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateScale_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector &InScale)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridScale(**FindGrid, Op, InScale);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateUniformScale_Implementation(const FIntVector& GridId, ETransformOp Op, float Scale)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridUniformScale(**FindGrid, Op, Scale);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateShear_Implementation(const FIntVector& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridShear(**FindGrid, Op, Shear, Axis0, Axis1);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateMatrix4dMultiply_Implementation(const FIntVector& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridMatrix4dMultiply(**FindGrid, Op, InX, InY, InZ, InW);
//	}
//}
//
//void AVoxelDatabase::InsertCoordinateMatrix3dMultiply_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ)
//{
//	const openvdb::GridBase::Ptr* FindGrid = GridData.Find(GridId);
//	if (FindGrid != nullptr)
//	{
//		VoxelDatabaseStatics::AddGridMatrix3dMultiply(**FindGrid, Op, InX, InY, InZ);
//	}
//}
//
