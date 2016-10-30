#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeSpecifier.generated.h"

USTRUCT()
struct FVoxelDatabaseTypeSpecifier
{
    GENERATED_BODY()

    FVoxelDatabaseTypeSpecifier()
        : Type((EVoxelDatabaseType)0), Name(TEXT("")), NameCount(-1)
    {}

    FVoxelDatabaseTypeSpecifier(EVoxelDatabaseType SpecifiedType, const FString& SpecifiedName)
        : Type(SpecifiedType), Name(SpecifiedName)
    {}

    FVoxelDatabaseTypeSpecifier(const FVoxelDatabaseTypeSpecifier& Other)
        : Type(Other.Type), Name(Other.Name), NameCount(Other.NameCount)
    {}

    FVoxelDatabaseTypeSpecifier(FVoxelDatabaseTypeSpecifier&& Other)
        : Type(Other.Type), Name(Other.Name), NameCount(Other.NameCount)
    {
        Other.Type = (EVoxelDatabaseType)0;
        Other.Name = TEXT("");
        Other.NameCount = -1;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EVoxelDatabaseType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 NameCount;

    FString GetTypeName() const
    {
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(Type);
        return TypeName;
    }

    FVoxelDatabaseTypeSpecifier& operator=(const FVoxelDatabaseTypeSpecifier& Other)
    {
        Type = Other.Type;
        Name = Other.Name;
        NameCount = Other.NameCount;
        return *this;
    }
};
