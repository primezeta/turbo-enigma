#pragma once
#include "VoxelValueSourceInterfaceVector2D.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelVector2DSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelVector2DSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FVector2D& OutValue) const;
};
