#pragma once
#include "VoxelValueSourceInterfaceUInt32.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUInt32SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUInt32SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, uint32& OutValue) const = 0;
};
