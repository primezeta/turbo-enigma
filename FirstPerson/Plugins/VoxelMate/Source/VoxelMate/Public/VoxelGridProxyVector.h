#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyVector.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FVector& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FVector& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector& InValue) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector& InValue, const bool& InIsActive) const;
};
