#pragma once
#include "VoxelValueSourceInterfaceUint32.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUint32SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUint32SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, uint32& OutValue) = 0;
};
