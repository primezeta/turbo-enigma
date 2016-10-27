#include "VoxelMatePrivatePCH.h"
#include "ArchiveGrid.h"

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeDisplayNames;
TMap<FString, int32> FGridFactory::TopologySizeByTreeType;
