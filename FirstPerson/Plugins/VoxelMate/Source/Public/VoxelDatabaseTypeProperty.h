#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "TypedProperty.h"
#include "VoxelDatabaseTypeProperty.generated.h"

UCLASS(ClassGroup = VoxelMate)
class UVoxelDatabaseTypeProperty : public UObject
{
    GENERATED_BODY()

public:
    UVoxelDatabaseTypeProperty(const FObjectInitializer& ObjectInitializer)
    {}

    UPROPERTY()
        FTypedProperty TypedProperty;
};
