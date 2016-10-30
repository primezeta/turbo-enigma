#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.h"
#include "VoxelDatabaseTransformMapTypeSpecifier.generated.h"

USTRUCT(BlueprintType)
struct FVoxelDatabaseTransformMapTypeSpecifier : public FVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

        FVoxelDatabaseTransformMapTypeSpecifier()
        : FVoxelDatabaseTypeSpecifier()
    {}

    FVoxelDatabaseTransformMapTypeSpecifier(EVoxelDatabaseType SpecifiedType, const FString& SpecifiedName)
        : FVoxelDatabaseTypeSpecifier(SpecifiedType, SpecifiedName)
    {}

    FVoxelDatabaseTransformMapTypeSpecifier(const FVoxelDatabaseMetadataTypeSpecifier& Other)
        : FVoxelDatabaseTypeSpecifier(Other)
    {}

    FVoxelDatabaseTransformMapTypeSpecifier(FVoxelDatabaseTransformMapTypeSpecifier&& Other)
        : FVoxelDatabaseTypeSpecifier(Other)
    {}

    FVoxelDatabaseTransformMapTypeSpecifier& operator=(const FVoxelDatabaseTransformMapTypeSpecifier& Other)
    {
        Type = Other.Type;
        Name = Other.Name;
        NameCount = Other.NameCount;
        return *this;
    }
};
