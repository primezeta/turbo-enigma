#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyIntPoint.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyIntPoint : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyIntPoint(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntPoint& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntPoint& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FIntPoint& InValue) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FIntPoint& InValue, const bool& InIsActive) const;
};
