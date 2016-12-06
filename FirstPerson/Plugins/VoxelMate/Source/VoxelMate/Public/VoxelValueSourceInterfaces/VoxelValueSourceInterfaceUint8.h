#pragma once
#include "VoxelValueSourceInterfaceUint8.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelUint8SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUint8SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, uint8& OutValue);
};
