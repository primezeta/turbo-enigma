#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"

FArchive& operator<<(FArchive& Ar, UVoxelDatabaseProxy& DatabaseProxy)
{
    if (!DatabaseProxy.IsDefaultSubobject())
    {
        Ar << DatabaseProxy.GridProxies;
    }
    return Ar;
}

void UVoxelDatabaseProxy::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    Ar << *this;
}

void UVoxelDatabaseProxy::PostInitProperties()
{
    Super::PostInitProperties();
}

bool UVoxelDatabaseProxy::IsReadyForFinishDestroy()
{
    //TODO: Return true when UVoxelDatabase async serialization is complete
    return Super::IsReadyForFinishDestroy();
}

void UVoxelDatabaseProxy::BeginDestroy()
{
    //TODO: Serialize UVoxelDatabase asyncronously
    Super::BeginDestroy();
}

void UVoxelDatabaseProxy::PreSave(const class ITargetPlatform* TargetPlatform)
{
    Super::PreSave(TargetPlatform);
}

void UVoxelDatabaseProxy::PostLoad()
{
    Super::PostLoad();
}

UVoxelDatabaseProxy* UVoxelDatabaseProxy::OpenDatabaseProxy()
{
    return UVoxelDatabase::Get().GetDatabaseProxy();
}

void UVoxelDatabaseProxy::AddGridBool(const FText& InGridName)
{
    FGuid GridId;
    if (UVoxelDatabase::Get().AddGrid<bool>(InGridName, false, GridId))
    {
        //MarkPackageDirty();
        AVoxelGridProxy* GridProxy = NewObject<AVoxelGridProxy>(this);
        check(GridProxy);
        GridProxy->GridId = GridId;
        GridProxy->GridDisplayText = InGridName;
        GridProxies.Add(GridProxy);
    }
}
