#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.h"
#include "VoxelDatabaseMetadataTypeSpecifier.generated.h"

USTRUCT(BlueprintType)
struct FVoxelDatabaseMetadataTypeSpecifier : public FVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

    FVoxelDatabaseMetadataTypeSpecifier()
        : FVoxelDatabaseTypeSpecifier()
    {}

    FVoxelDatabaseMetadataTypeSpecifier(EVoxelDatabaseType SpecifiedType, const FString& SpecifiedName)
        : FVoxelDatabaseTypeSpecifier(SpecifiedType, SpecifiedName)
    {}

    FVoxelDatabaseMetadataTypeSpecifier(const FVoxelDatabaseMetadataTypeSpecifier& Other)
        : FVoxelDatabaseTypeSpecifier(Other)
    {}

    FVoxelDatabaseMetadataTypeSpecifier(FVoxelDatabaseMetadataTypeSpecifier&& Other)
        : FVoxelDatabaseTypeSpecifier(Other)
    {}

    FVoxelDatabaseMetadataTypeSpecifier& operator=(const FVoxelDatabaseMetadataTypeSpecifier& Other)
    {
        Type = Other.Type;
        Name = Other.Name;
        NameCount = Other.NameCount;
        return *this;
    }
};
