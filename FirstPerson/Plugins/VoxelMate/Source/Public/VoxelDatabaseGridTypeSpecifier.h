#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.h"
#include "VoxelDatabaseGridTypeSpecifier.generated.h"

USTRUCT(BlueprintType)
struct FVoxelDatabaseGridTypeSpecifier : public FVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

    FVoxelDatabaseGridTypeSpecifier()
        : FVoxelDatabaseTypeSpecifier(), SaveFloatAsHalf(false)
    {}

    FVoxelDatabaseGridTypeSpecifier(EVoxelDatabaseType SpecifiedType, const FString& SpecifiedName, bool IsFloatSavedAsHalf)
        : FVoxelDatabaseTypeSpecifier(SpecifiedType, SpecifiedName), SaveFloatAsHalf(IsFloatSavedAsHalf)
    {}

    FVoxelDatabaseGridTypeSpecifier(const FVoxelDatabaseGridTypeSpecifier& Other)
        : FVoxelDatabaseTypeSpecifier(Other), SaveFloatAsHalf(Other.SaveFloatAsHalf)
    {}

    FVoxelDatabaseGridTypeSpecifier(FVoxelDatabaseGridTypeSpecifier&& Other)
        : FVoxelDatabaseTypeSpecifier(Other), SaveFloatAsHalf(Other.SaveFloatAsHalf)
    {
        Other.SaveFloatAsHalf = false;
    }

    FVoxelDatabaseGridTypeSpecifier& operator=(const FVoxelDatabaseGridTypeSpecifier& Other)
    {
        Type = Other.Type;
        Name = Other.Name;
        NameCount = Other.NameCount;
        SaveFloatAsHalf = Other.SaveFloatAsHalf;
        return *this;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool SaveFloatAsHalf;
};
