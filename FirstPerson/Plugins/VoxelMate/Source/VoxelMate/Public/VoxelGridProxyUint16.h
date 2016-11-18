#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt16.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt16 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint16 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const uint16& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const uint16& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint16& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint16& InValue, const bool& InIsActive);
};
