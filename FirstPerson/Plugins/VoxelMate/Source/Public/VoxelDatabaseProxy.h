#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseGridTypeSpecifier.h"
#include "VoxelDatabaseMetadataTypeSpecifier.h"
#include "VoxelDatabaseTransformMapTypeSpecifier.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Blueprintable) //TODO specifiers
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FTypedProperty> Grids;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FTypedProperty> Metadata;

    virtual void Serialize(FArchive& Ar) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        Super::PostEditChangeProperty(PropertyChangedEvent);
        //UProperty* Property = PropertyChangedEvent.Property;
        //const FString& PropertyName = Property->GetName();
        //if (PropertyName == TEXT("Grids"))
        //{
        //    const int32 Index = PropertyChangedEvent.GetArrayIndex(TEXT("Grids"));
        //    const EVoxelDatabaseType Type = Grids[Index];
        //}
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
    }
#endif // WITH_EDITOR
};
