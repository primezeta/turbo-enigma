#pragma once
#include "VoxelDatabaseCommonPrivate.h"

struct FMetaMapFactory
{
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap);
};
