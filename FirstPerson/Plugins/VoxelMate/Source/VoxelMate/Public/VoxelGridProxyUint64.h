#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt64.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint32 is not a supported blueprint type
    UFUNCTION(Category = VoxelMate)
        const uint64& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const uint64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint64& InValue) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint64& InValue, const bool& InIsActive) const;
};
