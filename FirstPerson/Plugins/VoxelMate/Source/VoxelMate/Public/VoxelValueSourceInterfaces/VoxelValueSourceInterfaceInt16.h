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
    UFUNCTION(Category = VoxelMate)
        virtual void GetValue(float x, float y, float z, int16& OutValue) = 0;
};
