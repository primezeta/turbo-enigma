#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"

void UVoxelDatabaseProxy::Serialize(FArchive& Ar)
{
    Ar << ProxyName;
    Ar << DatabaseName;
    FVoxelMateModule::SerializeDatabase(this, Ar);
}

UVoxelGridProxy* UVoxelDatabaseProxy::GetGridProxy(const FGuid& GridId)
{
    UVoxelGridProxy* GridProxy = GridProxies.FindOrAdd(GridId);
    if (GridProxy == nullptr)
    {
        //TODO need to add this to GC tree?
        GridProxy = NewObject<UVoxelGridProxy>(this);
    }

    //if (GridProxy->GridMeshComponent == nullptr)
    //{
        //GridProxy->GridMeshComponent = NewObject<UProceduralMeshComponent>(GridProxy);
    //}
    return GridProxy;
}
