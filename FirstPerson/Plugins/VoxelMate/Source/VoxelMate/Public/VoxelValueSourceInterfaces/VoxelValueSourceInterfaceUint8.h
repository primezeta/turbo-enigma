#pragma once
#include "VoxelValueSourceInterfaceUInt8.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelUInt8SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUInt8SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, uint8& OutValue);
};
