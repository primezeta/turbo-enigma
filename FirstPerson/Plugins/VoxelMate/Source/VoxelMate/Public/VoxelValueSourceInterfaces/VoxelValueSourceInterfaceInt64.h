#pragma once
#include "VoxelValueSourceInterfaceInt64.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelInt64SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelInt64SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate, meta = (CannotImplementInterfaceInBlueprint))
        virtual void GetValue(float x, float y, float z, int64& OutValue) const = 0;
    virtual void GetValue_Implementation(float x, float y, float z, int64& OutValue) const = 0;
};
