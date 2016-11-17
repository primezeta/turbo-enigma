#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyPackedRGBA16N.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedRGBA16N : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedRGBA16N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGBA16N& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGBA16N& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedRGBA16N& InValue) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedRGBA16N& InValue, const bool& InIsActive) const;
};
