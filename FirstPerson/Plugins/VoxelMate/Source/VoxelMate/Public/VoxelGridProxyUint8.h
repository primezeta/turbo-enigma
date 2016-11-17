#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt8.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const uint8& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const uint8& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint8& InValue) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint8& InValue, const bool& InIsActive) const;
};
