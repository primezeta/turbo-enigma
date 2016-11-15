#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "VoxelDatabase.h"
#include "VoxelGridProxyBool.h"

extern FVoxelDatabase VoxelDatabase;

const bool& UVoxelGridProxyBool::GetVoxelValue(const FIntVector& IndexCoord)
{
    check(FVoxelMateModule::IsLoaded());
    return VoxelDatabase.GetVoxelValue<bool>(GridId, IndexCoord);
}

const bool UVoxelGridProxyBool::GetVoxelIsActive(const FIntVector& IndexCoord)
{
    check(FVoxelMateModule::IsLoaded());
    return VoxelDatabase.GetVoxelIsActive<bool>(GridId, IndexCoord);
}

const bool& UVoxelGridProxyBool::GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive)
{
    check(FVoxelMateModule::IsLoaded());
    return VoxelDatabase.GetVoxelValue<bool>(GridId, IndexCoord, OutIsActive);
}

void UVoxelGridProxyBool::SetVoxelValue(const FIntVector& IndexCoord, const bool& InValue)
{
    check(FVoxelMateModule::IsLoaded());
    VoxelDatabase.SetVoxelValue<bool>(GridId, IndexCoord, InValue);
}

void UVoxelGridProxyBool::SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive)
{
    check(FVoxelMateModule::IsLoaded());
    VoxelDatabase.SetVoxelIsActive<bool>(GridId, IndexCoord, InIsActive);
}

void UVoxelGridProxyBool::SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const bool& InValue, const bool& InIsActive)
{
    check(FVoxelMateModule::IsLoaded());
    VoxelDatabase.SetVoxelValue<bool>(GridId, IndexCoord, InValue, InIsActive);
}
