#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt8.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION()
        const uint8& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const uint8& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const uint8& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint8& InValue, const bool& InIsActive);
};
