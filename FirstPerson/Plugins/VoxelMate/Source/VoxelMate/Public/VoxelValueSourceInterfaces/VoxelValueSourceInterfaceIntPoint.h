#pragma once
#include "VoxelValueSourceInterfaceIntPoint.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelIntPointSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelIntPointSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FIntPoint& OutValue) const;
};
