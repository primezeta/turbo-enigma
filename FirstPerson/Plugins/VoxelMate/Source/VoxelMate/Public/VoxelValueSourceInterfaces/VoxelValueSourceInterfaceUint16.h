#pragma once
#include "VoxelValueSourceInterfaceUint16.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUint16SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUint16SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, uint16& OutValue) = 0;
};
