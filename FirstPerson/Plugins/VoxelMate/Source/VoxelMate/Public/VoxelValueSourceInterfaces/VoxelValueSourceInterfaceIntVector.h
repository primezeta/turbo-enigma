#pragma once
#include "VoxelValueSourceInterfaceIntVector.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelIntVectorSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelIntVectorSourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FIntVector& OutValue);
};
