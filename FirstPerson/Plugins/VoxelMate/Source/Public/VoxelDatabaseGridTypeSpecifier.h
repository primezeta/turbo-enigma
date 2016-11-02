#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.h"
#include "VoxelDatabaseGridTypeSpecifier.generated.h"

UCLASS(ClassGroup = VoxelMate, Blueprintable, HideCategories=(VoxelValue, MetadataValue, TransformMapValue))
class UVoxelDatabaseGridTypeSpecifier : public UVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

public:
    UVoxelDatabaseGridTypeSpecifier(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
        SaveFloatAsHalf = false;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool SaveFloatAsHalf;
};
