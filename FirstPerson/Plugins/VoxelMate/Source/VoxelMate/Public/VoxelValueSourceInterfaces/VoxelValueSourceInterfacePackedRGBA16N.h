#pragma once
#include "VoxelValueSourceInterfacePackedRGBA16N.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelPackedRGBA16NSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelPackedRGBA16NSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FPackedRGBA16N& OutValue) const;
};
