#pragma once
#include "VoxelValueSourceInterfaceInt8.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelInt8SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelInt8SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, int8& OutValue) const = 0;
};
