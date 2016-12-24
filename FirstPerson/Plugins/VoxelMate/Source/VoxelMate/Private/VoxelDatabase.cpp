#include "VoxelMatePCH.h"
#include "UnrealNetwork.h"
#include "VoxelDatabase.h"
#include "ArchiveTransformMap.h"

void AVoxelDatabase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelDatabase, GridBaseValuesGenerators);
}
