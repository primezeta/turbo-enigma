#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyBool.generated.h"

UCLASS(ClassGroup = VoxelMate, NotBlueprintable, NotPlaceable)
class UVoxelGridProxyBool : public UVoxelGridProxy
{
    GENERATED_BODY()

public:
    UVoxelGridProxyBool(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
        if (GridMeshComponent == nullptr)
        {
            GridMeshComponent = NewObject<UProceduralMeshComponent>(this);
        }
    }

    UFUNCTION()
        const bool& GetVoxelValue(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool GetVoxelIsActive(const FIntVector& IndexCoord);
    UFUNCTION()
        const bool& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive);
    UFUNCTION()
        void SetVoxelValue(const FIntVector& IndexCoord, const bool& InValue);
    UFUNCTION()
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION()
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const bool& InValue, const bool& InIsActive);
};
