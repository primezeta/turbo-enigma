#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyFloat.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyFloat(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION()
        const float& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const float& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const float& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const float& InValue, const bool& InIsActive);
};
