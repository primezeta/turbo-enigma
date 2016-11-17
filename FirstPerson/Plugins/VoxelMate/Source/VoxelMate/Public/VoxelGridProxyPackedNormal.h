#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyPackedNormal.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedNormal : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedNormal(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FPackedNormal& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FPackedNormal& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedNormal& InValue) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedNormal& InValue, const bool& InIsActive) const;
};
