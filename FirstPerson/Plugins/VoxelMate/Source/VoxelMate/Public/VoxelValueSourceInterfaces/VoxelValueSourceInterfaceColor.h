#pragma once
#include "VoxelValueSourceInterfaceColor.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelColorSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelColorSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FColor& OutValue);
};
