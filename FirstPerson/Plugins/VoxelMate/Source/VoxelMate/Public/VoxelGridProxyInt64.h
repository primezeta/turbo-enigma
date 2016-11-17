#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyInt64.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION()
        const int64& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const int64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const int64& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int64& InValue, const bool& InIsActive);
};
