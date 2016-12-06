#pragma once
#include "VoxelValueSourceInterfaceVector4.generated.h"

UINTERFACE(Blueprintable)
class VOXELMATE_API UVoxelVector4SourceInterface : public UInterface
{
    GENERATED_BODY()
};

class VOXELMATE_API IVoxelVector4SourceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = VoxelMate)
        void GetValue(float x, float y, float z, FVector4& OutValue);
};
