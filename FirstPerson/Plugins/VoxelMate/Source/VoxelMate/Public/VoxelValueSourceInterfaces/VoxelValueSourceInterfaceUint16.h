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
    UFUNCTION(Category = VoxelMate, meta = (CannotImplementInterfaceInBlueprint))
        virtual void GetValue(float x, float y, float z, uint16& OutValue) const = 0;
    virtual void GetValue_Implementation(float x, float y, float z, uint16& OutValue) const = 0;
};
