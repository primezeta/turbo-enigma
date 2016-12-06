#pragma once
#include "VoxelValueSourceInterfaceLinearColor.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelLinearColorSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelLinearColorSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FLinearColor& OutValue) const;
};
