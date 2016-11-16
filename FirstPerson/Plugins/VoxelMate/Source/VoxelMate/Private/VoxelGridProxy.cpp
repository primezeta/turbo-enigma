#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelGridProxyBool.h"
#include "VoxelDatabase.h"

FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy)
{
    if (!GridProxy.IsDefaultSubobject())
    {
        Ar << GridProxy.GridId;
        Ar << GridProxy.GridDisplayText;
    }
    return Ar;
}

void AVoxelGridProxy::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    Ar << *this;
}

const bool& AVoxelGridProxyBool::GetVoxelValue(const FIntVector& IndexCoord)
{
    return UVoxelDatabase::Get().GetVoxelValue<bool>(GridId, IndexCoord);
}

const bool AVoxelGridProxyBool::GetVoxelIsActive(const FIntVector& IndexCoord)
{
    return UVoxelDatabase::Get().GetVoxelIsActive<bool>(GridId, IndexCoord);
}

const bool& AVoxelGridProxyBool::GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive)
{
    return UVoxelDatabase::Get().GetVoxelValue<bool>(GridId, IndexCoord, OutIsActive);
}

void AVoxelGridProxyBool::SetVoxelValue(const FIntVector& IndexCoord, const bool& InValue)
{
    UVoxelDatabase::Get().SetVoxelValue<bool>(GridId, IndexCoord, InValue);
}

void AVoxelGridProxyBool::SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive)
{
    UVoxelDatabase::Get().SetVoxelIsActive<bool>(GridId, IndexCoord, InIsActive);
}

void AVoxelGridProxyBool::SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const bool& InValue, const bool& InIsActive)
{
    UVoxelDatabase::Get().SetVoxelValue<bool>(GridId, IndexCoord, InValue, InIsActive);
}
