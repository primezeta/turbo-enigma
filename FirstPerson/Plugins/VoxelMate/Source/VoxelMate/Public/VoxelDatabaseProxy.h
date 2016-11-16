#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, CustomConstructor)
class VOXELMATE_API UVoxelDatabaseProxy : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
        TArray<AVoxelGridProxy*> GridProxies;
    //UPROPERTY()
        //UVoxelMetadataProxy* DatabaseMetadataProxy; //TODO

    friend FArchive& operator<<(FArchive& Ar, UVoxelDatabaseProxy& DatabaseProxy);
    virtual void Serialize(FArchive& Ar) override;
    virtual void PostInitProperties() override;
    virtual bool IsReadyForFinishDestroy() override;
    virtual void BeginDestroy() override;
    virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
    virtual void PostLoad() override;

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        static UVoxelDatabaseProxy* OpenDatabaseProxy();

    UFUNCTION(Category = VoxelMate, BlueprintCallable)
        void AddGridBool(const FText& InGridName);
};
