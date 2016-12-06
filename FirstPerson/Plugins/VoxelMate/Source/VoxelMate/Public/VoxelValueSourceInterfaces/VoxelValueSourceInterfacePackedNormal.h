#pragma once
#include "VoxelValueSourceInterfacePackedNormal.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelPackedNormalSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelPackedNormalSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FPackedNormal& OutValue);
};
