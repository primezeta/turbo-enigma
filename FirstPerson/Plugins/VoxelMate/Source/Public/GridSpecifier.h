#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "GridSpecifier.generated.h"

USTRUCT(BlueprintType)
struct FGridSpecifier
{
    GENERATED_BODY()

    UPROPERTY()
        EVoxelDatabaseType Type;

    UPROPERTY()
        FString Name;

    UPROPERTY()
        bool SaveFloatAsHalf;
};
