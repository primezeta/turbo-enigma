#pragma once
#include "VoxelValueSourceInterfacePackedRGB10A2N.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelPackedRGB10A2NSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelPackedRGB10A2NSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FPackedRGB10A2N& OutValue);
};
