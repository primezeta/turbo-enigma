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
    UFUNCTION(Category = VoxelMate)
        const double& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const double& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValue(const FIntVector& IndexCoord, const double& InValue) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const double& InValue, const bool& InIsActive) const;
};
