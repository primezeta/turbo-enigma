#pragma once
#include "EngineMinimal.h"
//#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
//#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
//#include "PhysicsPublic.h"
#include "ProceduralMeshComponent.h"
#include "VoxelGridProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Abstract, NotBlueprintable, NotPlaceable)
class UVoxelGridProxy : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
        FGuid GridId;
    UPROPERTY()
        FText GridDisplayText;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY()
        UProceduralMeshComponent* GridMeshComponent;
};
