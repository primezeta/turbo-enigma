#pragma once
#include "VoxelValueSourceInterfaceInt16.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelInt16SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelInt16SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z);
};
