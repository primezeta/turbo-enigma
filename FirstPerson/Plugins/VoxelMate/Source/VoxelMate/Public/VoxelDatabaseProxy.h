#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, CustomConstructor, Blueprintable)
class VOXELMATE_API UVoxelDatabaseProxy : public UObject
{
    GENERATED_BODY()

public:
    friend FArchive& operator<<(FArchive& Ar, UVoxelDatabaseProxy& DatabaseProxy);
    virtual void Serialize(FArchive& Ar) override;
    virtual void PostInitProperties() override;
    virtual bool IsReadyForFinishDestroy() override;
    virtual void BeginDestroy() override;
    virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
    virtual void PostLoad() override;

    UPROPERTY(BlueprintReadOnly)
        TArray<AVoxelGridProxy*> GridProxies;
    //UPROPERTY()
        //UVoxelMetadataProxy* DatabaseMetadataProxy; //TODO
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        static UVoxelDatabaseProxy* OpenDatabaseProxy();
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddGridProxy(EVoxelDatabaseVoxelType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf);
};
