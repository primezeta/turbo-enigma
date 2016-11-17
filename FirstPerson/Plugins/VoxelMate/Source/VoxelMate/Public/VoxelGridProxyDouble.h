#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyDouble.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyDouble : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyDouble(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //double is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const double& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        const double& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const double& InValue) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const double& InValue, const bool& InIsActive) const;
};
