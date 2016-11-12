#pragma once
#include "EngineGridTypes.h"

struct FMetaMapFactory
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap);
};
