#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseTypeFactory.h"

FORCEINLINE FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap);
