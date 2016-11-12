#pragma once
#include "EngineMinimal.h"
#include "VoxelGridProxy.h"
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

    virtual void Serialize(FArchive& Ar) override;
    UVoxelGridProxy* GetGridProxy(const FGuid& GridId);
    UVoxelGridProxy* ActiveGrid;

private:
    TMap<FGuid, UVoxelGridProxy*> GridProxies;
};
