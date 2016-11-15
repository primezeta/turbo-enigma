#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, NotBlueprintable, NotPlaceable, CustomConstructor)
class UVoxelDatabaseProxy : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
        FString DatabaseName;
    UPROPERTY()
        TArray<UVoxelGridProxy*> GridProxies;
    //UPROPERTY()
        //UVoxelMetadataProxy* DatabaseMetadataProxy; //TODO

    virtual void Serialize(FArchive& Ar) override;

    UFUNCTION()
        void AddGridBool(const FText& InGridName);
};
