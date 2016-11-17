#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelGridProxyVector4.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector4 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector4(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FVector4& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const bool GetVoxelIsActive(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        const FVector4& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector4& InValue) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive) const;
    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector4& InValue, const bool& InIsActive) const;
};
