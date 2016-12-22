#include "VoxelMatePCH.h"
#include "UnrealNetwork.h"
#include "VoxelDatabase.h"
#include "VoxelDatabaseProxy.h"
#include "VoxelGridProxy.h"

AVoxelDatabaseProxy::AVoxelDatabaseProxy()
{
    SetReplicates(true);
    SetReplicateMovement(false);
    SetActorEnableCollision(false);
    bNetTemporary = 0;
    bAutoDestroyWhenFinished = 1;
    bCanBeDamaged = 0;
}

AVoxelDatabaseProxy* AVoxelDatabaseProxy::InitVoxelDatabase()
{
	if (!VoxelDatabase)
	{
		VoxelDatabase = NewObject<AVoxelDatabase>();
		//Database->IsSafeForRootSet() TODO
		VoxelDatabase->AddToRoot();
	}

	AVoxelDatabaseProxy* DatabaseProxy = nullptr;
	if (VoxelDatabase->GetNetMode() != ENetMode::NM_Client)
	{
		if (!VoxelDatabase->AuthDatabaseProxy)
		{
			DatabaseProxy = NewObject<AVoxelDatabaseProxy>(VoxelDatabase);
			VoxelDatabase->DatabaseProxy = DatabaseProxy;
			VoxelDatabase->AuthDatabaseProxy = DatabaseProxy;
		}
		else
		{
			DatabaseProxy = VoxelDatabase->LocalDatabaseProxy;
		}
	}
	return DatabaseProxy;
}

void AVoxelDatabaseProxy::LoadVoxelDatabase(const FString& DatabaseDirectory)
{
	if (VoxelDatabase)
	{
		FArchive* Reader = IFileManager::Get().CreateFileReader(*DatabaseDirectory);
		if (Reader)
		{
			VoxelDatabase->Serialize(*Reader);
		}
	}
}

void AVoxelDatabaseProxy::SaveVoxelDatabase(const FString& DatabaseDirectory)
{
	if (VoxelDatabase)
	{
		FArchive* Writer = IFileManager::Get().CreateFileWriter(*DatabaseDirectory);
		if (Writer)
		{
			VoxelDatabase->Serialize(*Writer);
		}
	}
}

bool AVoxelDatabaseProxy::IsReadyForFinishDestroy()
{
    //TODO: Return true when AVoxelDatabase async serialization is complete
    return Super::IsReadyForFinishDestroy();
}

void AVoxelDatabaseProxy::BeginDestroy()
{
    //TODO: Serialize AVoxelDatabase asyncronously
    Super::BeginDestroy();
}

void AVoxelDatabaseProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelDatabaseProxy, GridProxies);
}

AVoxelGridProxy* AVoxelDatabaseProxy::AddScalarGrid(EVoxelScalarType VoxelType, const FText& GridDisplayText)
{
    if (!VoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelScalarType::Bool && VoxelDatabase->AddGrid<FVoxelBool>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyBool>(VoxelDatabase);
    }
    else if (VoxelType == EVoxelScalarType::UInt8 && VoxelDatabase->AddGrid<FVoxelUInt8>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt8>(VoxelDatabase);
    }
    else if (VoxelType == EVoxelScalarType::Int32 && VoxelDatabase->AddGrid<FVoxelInt32>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt32>(VoxelDatabase);
    }

    if (GridProxy)
    {
        check(GridId.IsValid());
        GridProxy->GridId = GridId;
        GridProxy->GridDisplayText = GridDisplayText;
        GridProxies.Add(GridProxy);
    }
    else
    {
        //TODO log error (failed to create grid proxy)
    }
    return GridProxy;
}

AVoxelGridProxy* AVoxelDatabaseProxy::AddFloatScalarGrid(EVoxelFloatScalarType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    if (!VoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatScalarType::Float && VoxelDatabase->AddGrid<FVoxelFloat>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
		AVoxelGridProxyFloat* GridProxyFloat = NewObject<AVoxelGridProxyFloat>(VoxelDatabase);
		if (GridProxyFloat)
		{
			check(GridId.IsValid());
			GridProxyFloat->GridId = GridId;
			GridProxyFloat->IsFloatSavedAsHalf = SaveFloatAsHalf;
			GridProxyFloat->GridDisplayText = GridDisplayText;
			GridProxy = GridProxyFloat;
			GridProxies.Add(GridProxyFloat);
		}
    }

    return GridProxy;
}

AVoxelGridProxy* AVoxelDatabaseProxy::AddVectorGrid(EVoxelVectorType VoxelType, const FText& GridDisplayText)
{
    if (!VoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

	if (VoxelType == EVoxelVectorType::IntVector && VoxelDatabase->AddGrid<FVoxelIntVector>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntVector>(VoxelDatabase);
    }

    if (GridProxy)
    {
        check(GridId.IsValid());
        GridProxy->GridId = GridId;
        GridProxy->GridDisplayText = GridDisplayText;
        GridProxies.Add(GridProxy);
    }
    else
    {
        //TODO log error (failed to create grid proxy)
    }
    return GridProxy;
}

AVoxelGridProxy* AVoxelDatabaseProxy::AddFloatVectorGrid(EVoxelFloatVectorType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    if (!VoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatVectorType::Vector && VoxelDatabase->AddGrid<FVoxelVector>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
		AVoxelGridProxyVector* GridProxyVector = NewObject<AVoxelGridProxyVector>(VoxelDatabase);
		if (GridProxyVector)
		{
			check(GridId.IsValid());
			GridProxyVector->GridId = GridId;
			GridProxyVector->IsFloatSavedAsHalf = SaveFloatAsHalf;
			GridProxyVector->GridDisplayText = GridDisplayText;
			GridProxy = GridProxyVector;
			GridProxies.Add(GridProxyVector);
		}
    }

    return GridProxy;
}
