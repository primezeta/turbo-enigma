#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"

extern FVoxelDatabase VoxelDatabase;

void UVoxelDatabaseProxy::Serialize(FArchive& Ar)
{
    Ar << DatabaseName;
    FVoxelMateModule::GetChecked().SerializeDatabase(Ar);
}

void UVoxelDatabaseProxy::AddGridBool(const FText& InGridName)
{
    check(FVoxelMateModule::IsLoaded());
    FGuid GridId;
    if (VoxelDatabase.AddGrid<bool>(InGridName, false, GridId))
    {
        UVoxelGridProxy* GridProxy = NewObject<UVoxelGridProxyBool>(this);
        if (GridProxy)
        {
            GridProxies.Add(GridProxy);
        }
    }
}
