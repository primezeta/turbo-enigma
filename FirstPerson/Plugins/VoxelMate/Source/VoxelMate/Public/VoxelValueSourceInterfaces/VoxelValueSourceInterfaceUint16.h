#pragma once
#include "VoxelValueSourceInterfaceUInt16.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUInt16SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUInt16SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, uint16& OutValue) = 0;
};
