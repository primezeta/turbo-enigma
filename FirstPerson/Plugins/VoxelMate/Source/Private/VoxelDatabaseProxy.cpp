#include "VoxelMatePrivatePCH.h"
#include "VoxelMate.h"
#include "VoxelDatabaseProxy.h"

void UVoxelDatabaseProxy::Serialize(FArchive& Ar)
{
    Ar << ProxyName;
    Ar << DatabaseName;
    FVoxelMateModule::SerializeDatabase(this, Ar);
}
