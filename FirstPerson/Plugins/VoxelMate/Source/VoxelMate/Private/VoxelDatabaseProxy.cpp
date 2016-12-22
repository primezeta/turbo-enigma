#include "VoxelMatePCH.h"
#include "VoxelDatabase.h"
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
    if (!AVoxelDatabase::VoxelMateVoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelScalarType::Bool && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelBool>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyBool>(this);
    }
    else if (VoxelType == EVoxelScalarType::UInt8 && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelUInt8>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt8>(this);
    }
    else if (VoxelType == EVoxelScalarType::Int32 && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelInt32>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt32>(this);
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
    if (!AVoxelDatabase::VoxelMateVoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatScalarType::Float && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelFloat>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
		AVoxelGridProxyFloat* GridProxyFloat = NewObject<AVoxelGridProxyFloat>(this);
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
    if (!AVoxelDatabase::VoxelMateVoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

	if (VoxelType == EVoxelVectorType::IntVector && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelIntVector>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntVector>(this);
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
    if (!AVoxelDatabase::VoxelMateVoxelDatabase)
    {
        return nullptr;
    }

    FGuid GridId;
	AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatVectorType::Vector && AVoxelDatabase::VoxelMateVoxelDatabase->AddGrid<FVoxelVector>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
		AVoxelGridProxyVector* GridProxyVector = NewObject<AVoxelGridProxyVector>(this);
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
