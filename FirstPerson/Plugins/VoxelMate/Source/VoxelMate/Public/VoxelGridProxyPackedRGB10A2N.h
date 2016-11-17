#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyPackedRGB10A2N.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedRGB10A2N : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedRGB10A2N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGB10A2N& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGB10A2N& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedRGB10A2N& InValue) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedRGB10A2N& InValue, const bool& InIsActive) const;
};
