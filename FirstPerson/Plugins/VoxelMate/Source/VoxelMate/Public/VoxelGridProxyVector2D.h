#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyVector2D.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyVector2D : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector2D(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION()
        const FVector2D& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const FVector2D& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector2D& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector2D& InValue, const bool& InIsActive);
};
