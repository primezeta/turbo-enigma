#include "VoxelMatePCH.h"
#include "VoxelDatabase.h"
#include "VoxelDatabaseStatics.h"

TMap<FGuid, openvdb::GridBase::Ptr> ClientGridData;

void AVoxelDatabase::Serialize(FArchive& Ar)
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

TMap<FGuid, openvdb::GridBase::Ptr> GridChanges;

bool AVoxelDatabase::AddGrid_Validate(AValueSource* ValueSource, const FText& GridText)
{
	NET_LOG(this, LogVoxelMate, "%s", *GridText.ToString());

	return true; //TODO
}

void AVoxelDatabase::AddGrid_Implementation(AValueSource* ValueSource, const FText& GridText)
{
	NET_LOG(this, LogVoxelMate, "%s", *GridText.ToString());

	const ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Client)
	{
		return;
	}

	check(ValueSource);

	if (GridChanges.Contains(ValueSource->GridId))
	{
		return;
	}

	openvdb::GridBase::Ptr GridPtr = nullptr;
	switch (ValueSource->VoxelType)
	{
	case EVoxelType::Bool:
		GridPtr = FGridFactory::Create<FVoxelBool>();
		break;
	case EVoxelType::UInt8:
		GridPtr = FGridFactory::Create<FVoxelUInt8>();
		break;
	case EVoxelType::Int32:
		GridPtr = FGridFactory::Create<FVoxelInt32>();
		break;
	case EVoxelType::Float:
		GridPtr = FGridFactory::Create<FVoxelFloat>();
		break;
	case EVoxelType::IntVector:
		GridPtr = FGridFactory::Create<FVoxelIntVector>();
		break;
	case EVoxelType::Vector:
		GridPtr = FGridFactory::Create<FVoxelVector>();
		break;
	default:
		check(false);
	}

	check(GridPtr != nullptr);
	GridChanges.Add(ValueSource->GridId, GridPtr);
	GridBaseValuesGenerators.AddItem(FGridBaseValuesGeneratorItem(GridText, ValueSource));
}

bool AVoxelDatabase::ChangeVoxel_Validate(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive)
{
	NET_LOG(this, LogVoxelMate, "%s", *GridId.ToString());

	return true; //TODO
}

void AVoxelDatabase::ChangeVoxel_Implementation(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive)
{
	const ENetMode NetMode = GetNetMode();
	if (NetMode != NM_Client)
	{
		NET_LOG(this, LogVoxelMate, "%s (Auth)", *GridId.ToString());
		const openvdb::GridBase::Ptr* FindServerGrid = GridChanges.Find(GridId);
		VoxelDatabaseStatics::ChangeVoxelValue(*FindServerGrid, IndexCoord, Voxel, IsActive);
	}

	if (NetMode != ENetMode::NM_DedicatedServer)
	{
		NET_LOG(this, LogVoxelMate, "%s (Non-auth)", *GridId.ToString());
		const openvdb::GridBase::Ptr* FindClientGrid = ClientGridData.Find(GridId);
		VoxelDatabaseStatics::ChangeVoxelValue(*FindClientGrid, IndexCoord, Voxel, IsActive);
	}
}

void AVoxelDatabase::OnRep_GridBaseValuesGeneratorArray()
{
	NET_LOG(this, LogVoxelMate, "");

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		return;
	}

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
				VoxelDatabaseStatics::CreateGridData<FVoxelBool>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelBool, AVoxelBoolSource>(GridPtr, *Cast<AVoxelBoolSource>(Item.ValueSource));
				break;
			case EVoxelType::UInt8:
				VoxelDatabaseStatics::CreateGridData<FVoxelUInt8>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelUInt8, AVoxelUInt8Source>(GridPtr, *Cast<AVoxelUInt8Source>(Item.ValueSource));
				break;
			case EVoxelType::Int32:
				VoxelDatabaseStatics::CreateGridData<FVoxelInt32>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelInt32, AVoxelInt32Source>(GridPtr, *Cast<AVoxelInt32Source>(Item.ValueSource));
				break;
			case EVoxelType::Float:
				VoxelDatabaseStatics::CreateGridData<FVoxelFloat>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelFloat, AVoxelFloatSource>(GridPtr, *Cast<AVoxelFloatSource>(Item.ValueSource));
				break;
			case EVoxelType::IntVector:
				VoxelDatabaseStatics::CreateGridData<FVoxelIntVector>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelIntVector, AVoxelIntVectorSource>(GridPtr, *Cast<AVoxelIntVectorSource>(Item.ValueSource));
				break;
			case EVoxelType::Vector:
				VoxelDatabaseStatics::CreateGridData<FVoxelVector>(Item.GridId, Item.DisplayText, false, GridPtr);
				VoxelDatabaseStatics::CreateVoxelValuesFromSource<FVoxelVector, AVoxelVectorSource>(GridPtr, *Cast<AVoxelVectorSource>(Item.ValueSource));
				break;
			default:
				check(false);
			}

			ClientGridData.Add(Item.GridId, GridPtr);
		}
	}
}

void AVoxelDatabase::SetCoordinateTransformAffine_Implementation(const FGuid& GridId, const FAffineCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FAffineCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformUnitary_Implementation(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FUnitaryCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformScale_Implementation(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformUniformScale_Implementation(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformTranslation_Implementation(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformScaleTranslation_Implementation(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformUniformScaleTranslation_Implementation(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::SetCoordinateTransformNonlinearFrustum_Implementation(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr && *FindGrid != nullptr)
	{
		VoxelDatabaseStatics::SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(**FindGrid, InCoordinateTransform);
	}
}

void AVoxelDatabase::InsertGridRotation_Implementation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridRotation(**FindGrid, Op, Radians, Axis);
	}
}

void AVoxelDatabase::InsertGridTranslation_Implementation(const FGuid& GridId, ETransformOp Op, const FVector &InTranslation)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridTranslation(**FindGrid, Op, InTranslation);
	}
}

void AVoxelDatabase::InsertGridScale_Implementation(const FGuid& GridId, ETransformOp Op, const FVector &InScale)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridScale(**FindGrid, Op, InScale);
	}
}

void AVoxelDatabase::InsertGridUniformScale_Implementation(const FGuid& GridId, ETransformOp Op, float Scale)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridUniformScale(**FindGrid, Op, Scale);
	}
}

void AVoxelDatabase::InsertGridShear_Implementation(const FGuid& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridShear(**FindGrid, Op, Shear, Axis0, Axis1);
	}
}

void AVoxelDatabase::InsertGridMatrix4dMultiply_Implementation(const FGuid& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridMatrix4dMultiply(**FindGrid, Op, InX, InY, InZ, InW);
	}
}

void AVoxelDatabase::InsertGridMatrix3dMultiply_Implementation(const FGuid& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ)
{
	const openvdb::GridBase::Ptr* FindGrid = ClientGridData.Find(GridId);
	if (FindGrid != nullptr)
	{
		VoxelDatabaseStatics::AddGridMatrix3dMultiply(**FindGrid, Op, InX, InY, InZ);
	}
}

void AVoxelDatabase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelDatabase, GridBaseValuesGenerators);
}
