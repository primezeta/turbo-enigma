#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyUInt64.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint32 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const uint64& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const uint64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint64& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint64& InValue, const bool& InIsActive);
};
