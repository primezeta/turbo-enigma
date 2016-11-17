#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyInt8.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int8 is not a supported blueprint type
    UFUNCTION(Category = VoxelMate)
        const int8& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate)
        const int8& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValue(const FIntVector& IndexCoord, const int8& InValue) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int8& InValue, const bool& InIsActive) const;
};
