#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseGridTypeSpecifier.h"
#include "VoxelDatabaseMetadataTypeSpecifier.h"
#include "VoxelDatabaseTransformMapTypeSpecifier.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate) //TODO specifiers
class UVoxelDatabaseProxy : public UObject
{
    GENERATED_BODY()

public:
    UVoxelDatabaseProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FString ProxyName;
    UPROPERTY()
        FString DatabaseName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) //TODO investigate making Grids NonTransactional
        TArray<UVoxelDatabaseGridTypeSpecifier*> Grids;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<UVoxelDatabaseMetadataTypeSpecifier*> Metadata;

    FString Test;
    virtual void Serialize(FArchive& Ar) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        Test = PropertyChangedEvent.Property->GetMetaData(TEXT("HideCategories"));

        //if (PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Add       ||
        //    PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Duplicate ||
        //    PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Insert)
        //{
        //    const int32& Index = PropertyChangedEvent.GetArrayIndex();
        //    const FString& GridName = Grids[Index];
        //    AddGrid()
        //}
        //else if (PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Clear ||
        //         PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Delete)
        //{

        //}
        Super::PostEditChangeProperty(PropertyChangedEvent);
    }
#endif // WITH_EDITOR
};
