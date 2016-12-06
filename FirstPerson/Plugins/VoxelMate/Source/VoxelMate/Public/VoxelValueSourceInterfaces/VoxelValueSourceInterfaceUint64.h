#pragma once
#include "VoxelValueSourceInterfaceUint64.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUint64SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUint64SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, uint64& OutValue) = 0;
};
