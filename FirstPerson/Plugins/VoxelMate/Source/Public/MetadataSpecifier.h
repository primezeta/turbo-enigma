#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "MetadataSpecifier.generated.h"

USTRUCT(BlueprintType)
struct FMetadataSpecifier
{
    GENERATED_BODY()

    UPROPERTY()
        EVoxelDatabaseType Type;

    UPROPERTY()
        FString Name;
};
