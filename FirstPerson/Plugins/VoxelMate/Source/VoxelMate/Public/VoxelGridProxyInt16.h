#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyInt16.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt16 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int16 is not a supported blueprint type
    UFUNCTION(Category = VoxelMate)
        const int16& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const int16& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValue(const FIntVector& IndexCoord, const int16& InValue) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int16& InValue, const bool& InIsActive) const;
};
