#pragma once
#include "EngineMinimal.h"
//#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
//#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
//#include "PhysicsPublic.h"
#include "ProceduralMeshComponent.h"
#include "BulkData.h"
#include "VoxelGridProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelGridProxy : public AActor
{
    GENERATED_BODY()

public:
    AVoxelGridProxy(const FObjectInitializer& ObjectInitializer);

    friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy);
    virtual void Serialize(FArchive& Ar) override;

    UPROPERTY(BlueprintReadOnly)
        FGuid GridId;
    UPROPERTY(BlueprintReadOnly)
        bool IsFloatSavedAsHalf;
    UPROPERTY(BlueprintReadOnly)
        FText GridDisplayText;
    UPROPERTY(BlueprintReadOnly)
        FString DataFilePath;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY()
        UProceduralMeshComponent* GridMeshComponent;

    UFUNCTION()
        bool LoadVoxelData();
    UFUNCTION()
        bool SaveVoxelData();

protected:
    //TODO idea for faster grid lookup
    //int32 CacheIndex;
};
