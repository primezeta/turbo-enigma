#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "MetadataSpecifier.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FMetadataSpecifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EVoxelDatabaseType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Name;
};
