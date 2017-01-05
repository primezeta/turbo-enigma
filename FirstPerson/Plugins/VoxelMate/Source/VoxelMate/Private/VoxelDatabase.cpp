#include "VoxelMatePCH.h"
#include "VoxelDatabase.h"
#include "VoxelDatabaseStatics.h"

TMap<TSubclassOf<AGridSource>, openvdb::GridBase::Ptr> GridData;

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

bool AVoxelDatabase::AddGrid_Validate(TSubclassOf<AGridSource> GridSourceClass, const FText& GridText)
{
	NET_LOG(this, LogVoxelMate, "%s", *GridText.ToString());

	return true; //TODO
}

void AVoxelDatabase::AddGrid_Implementation(TSubclassOf<AGridSource> GridSourceClass, const FText& GridText)
{
	NET_LOG(this, LogVoxelMate, "%s", *GridText.ToString());

	if (!GridData.Contains(GridSourceClass))
	{
		UWorld* World = GetWorld();
		check(World);
		//AGridSource* GridSource = Cast<AGridSource>(UGameplayStatics::SpawnObject(GridSourceClass, this));
		AGridSource* GridSource = Cast<AGridSource>(World->SpawnActor(GridSourceClass));
		if (GridSource)
		{
			GridSource->SetOwner(this);
			GridSource->SetReplicates(true);
			GridSource->bAlwaysRelevant = true;

			openvdb::GridBase::Ptr GridPtr = nullptr;
			VoxelDatabaseStatics::CreateGridData(GridText, GridSource->IsFloatSavedAsHalf, GridPtr);
			GridData.Add(GridSourceClass, GridPtr);

			//const ENetMode NetMode = GetNetMode();
			//if (NetMode != ENetMode::NM_DedicatedServer)
			//{
			//	VoxelDatabaseStatics::CreateVoxelValuesFromSource(GridPtr, *GridSource);
			//}
		}
	}
}

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
//void AVoxelDatabase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(AVoxelDatabase, GridBaseValuesGenerators);
//}
