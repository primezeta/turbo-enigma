#pragma once
#include "VoxelValueSourceInterfaceFloat.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelFloatSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelFloatSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, float& OutValue);
};
