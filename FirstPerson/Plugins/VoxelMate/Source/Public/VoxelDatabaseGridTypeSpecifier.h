#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.h"
#include "VoxelDatabaseGridTypeSpecifier.generated.h"

UCLASS(BlueprintType)
class UVoxelDatabaseGridTypeSpecifier : public UVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool SaveFloatAsHalf;
};
