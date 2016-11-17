#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyFloat.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyFloat(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const float& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const float& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const float& InValue) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const float& InValue, const bool& InIsActive) const;
};
