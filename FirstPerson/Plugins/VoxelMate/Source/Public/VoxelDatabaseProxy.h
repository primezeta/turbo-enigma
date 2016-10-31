#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS() //TODO specifiers
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
};
