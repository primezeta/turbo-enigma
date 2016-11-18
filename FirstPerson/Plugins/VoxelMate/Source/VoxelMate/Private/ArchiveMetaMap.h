#pragma once
#include "VoxelDatabaseCommonPrivate.h"
#include "ArchiveMetaValue.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

struct FMetaMapFactory
{
    VOXELMATEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap);
    VOXELMATEINLINE bool Serialize(FArchive& Ar)
    {
        Ar << MetaMap;
        return true;
    }

    openvdb::MetaMap MetaMap;
};

#pragma warning(pop)
