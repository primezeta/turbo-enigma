#pragma once
#include "VoxelValueSourceInterfaceDouble.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelDoubleSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelDoubleSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate, meta = (CannotImplementInterfaceInBlueprint))
        virtual void GetValue(float x, float y, float z, double& OutValue) const = 0;
    virtual void GetValue_Implementation(float x, float y, float z, double& OutValue) const = 0;
};
