#pragma once
#include "VoxelValueSourceInterfaceBool.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelBoolSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelBoolSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, bool& OutValue) const;
};
