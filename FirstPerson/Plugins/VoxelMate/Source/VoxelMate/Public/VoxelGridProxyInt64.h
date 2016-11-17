#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyInt64.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int64 is not a supported blueprint type
    UFUNCTION(Category = VoxelMate)
        const int64& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const int64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValue(const FIntVector& IndexCoord, const int64& InValue) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int64& InValue, const bool& InIsActive) const;
};
