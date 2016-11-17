#pragma once
#include "EngineMinimal.h"
//#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
//#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
//#include "PhysicsPublic.h"
#include "ProceduralMeshComponent.h"
#include "VoxelGridProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelGridProxy : public AActor
{
    GENERATED_BODY()

public:
    AVoxelGridProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
        GridMeshComponent = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("GridMeshComponent"));
    }

    friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy);
    virtual void Serialize(FArchive& Ar) override;

    UPROPERTY()
        FGuid GridId;
    UPROPERTY()
        FText GridDisplayText;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY()
        UProceduralMeshComponent* GridMeshComponent;
};
