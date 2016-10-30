#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "GridSpecifier.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FGridSpecifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EVoxelDatabaseType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool SaveFloatAsHalf;
};
