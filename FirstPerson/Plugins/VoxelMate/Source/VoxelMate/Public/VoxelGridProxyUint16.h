#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt16.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyUInt16 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION()
        const uint16& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const uint16& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const uint16& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint16& InValue, const bool& InIsActive);
};
