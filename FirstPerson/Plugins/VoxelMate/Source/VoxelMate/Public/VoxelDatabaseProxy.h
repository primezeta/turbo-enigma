#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
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

    UVoxelDatabaseProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    static bool CreateGridDataFile(AVoxelGridProxy* GridProxy, FText& OutFailureReason);
    bool LoadGridData(AVoxelGridProxy* GridProxy);
    bool SaveGridData(AVoxelGridProxy* GridProxy);

    UPROPERTY(BlueprintReadOnly)
        TArray<AVoxelGridProxy*> GridProxies;
    //UPROPERTY()
        //UVoxelMetadataProxy* DatabaseMetadataProxy; //TODO
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        static UVoxelDatabaseProxy* OpenDatabaseProxy();
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddScalarGrid(EVoxelScalarType VoxelType, const FText& GridDisplayText);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddFloatScalarGrid(EVoxelFloatScalarType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddVectorGrid(EVoxelVectorType VoxelType, const FText& GridDisplayText);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddFloatVectorGrid(EVoxelFloatVectorType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf);
};
