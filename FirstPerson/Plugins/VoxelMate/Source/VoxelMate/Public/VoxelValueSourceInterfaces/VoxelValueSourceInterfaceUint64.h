#pragma once
#include "VoxelValueSourceInterfaceUInt64.generated.h"

UINTERFACE()
class VOXELMATE_API UVoxelUInt64SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelUInt64SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelMate, meta = (CannotImplementInterfaceInBlueprint))
        virtual void GetValue(float x, float y, float z, uint64& OutValue) const = 0;
    virtual void GetValue_Implementation(float x, float y, float z, uint64& OutValue) const = 0;
};
