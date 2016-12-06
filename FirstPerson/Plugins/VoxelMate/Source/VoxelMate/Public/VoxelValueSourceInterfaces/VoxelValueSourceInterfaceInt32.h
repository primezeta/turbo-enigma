#pragma once
#include "VoxelValueSourceInterfaceInt32.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelInt32SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelInt32SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, int32& OutValue);
};
