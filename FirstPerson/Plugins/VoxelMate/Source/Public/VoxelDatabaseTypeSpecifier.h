#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeProperty.h"
#include "VoxelDatabaseTypeSpecifier.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, DefaultToInstanced)
class UVoxelDatabaseTypeSpecifier : public UObject
{
    GENERATED_BODY()

public:
    UVoxelDatabaseTypeSpecifier(const FObjectInitializer& ObjectInitializer)
        : Type((EVoxelDatabaseType)0), Name(TEXT("")), NameCount(-1)
    {
        Type = (EVoxelDatabaseType)0;
        Name = TEXT("");
        NameCount = 0;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EVoxelDatabaseType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 NameCount;

    UPROPERTY(EditAnywhere)
        UVoxelDatabaseTypeProperty* VoxelDatabaseType;

    UFUNCTION()
        FString GetTypeName() const
        {
            const FString TypeName = EnumValueToString<EVoxelDatabaseType>(Type);
            return TypeName;
        }
};
