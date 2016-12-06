#pragma once
#include "VoxelValueSourceInterfaceVector.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelVectorSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelVectorSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FVector& OutValue) const;
};
