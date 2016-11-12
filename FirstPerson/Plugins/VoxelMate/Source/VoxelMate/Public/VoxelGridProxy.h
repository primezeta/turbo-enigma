#pragma once
#include "EngineMinimal.h"
//#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
//#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
//#include "PhysicsPublic.h"
//#include "ProceduralMeshComponent.h"
#include "VoxelGridProxy.generated.h"

//TODO
UCLASS()
class UVoxelGridProxy : public UObject
{
    GENERATED_BODY()

public:
    /**
    * Primitive data use to track which aggregate geometry is selected
    */
    //struct FPrimData
    //{
        //EKCollisionPrimitiveType		PrimType;
        //int32							PrimIndex;

        //FPrimData(EKCollisionPrimitiveType InPrimType, int32 InPrimIndex) :
        //    PrimType(InPrimType),
        //    PrimIndex(InPrimIndex) {}

        //bool operator==(const FPrimData& Other) const
        //{
        //    return (PrimType == Other.PrimType && PrimIndex == Other.PrimIndex);
        //}
    //};
    
    UVoxelGridProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
//#if WITH_EDITORONLY_DATA
//        ThumbnailInfo = USceneThumbnailInfo::StaticClass()->GetDefaultObject<USceneThumbnailInfo>();
//#endif
    }

    //UPROPERTY()
        //UProceduralMeshComponent* GridMeshComponent;

//#if WITH_EDITORONLY_DATA
//    /** Information on the selected collision primitives */
//    TArray<FPrimData> SelectedPrims;
//
//    //TODO SlateEditorStyle.cpp is where all kinds of different editor modules have styles like font, icons, etc set. How to extend that for the VoxelMate plugin?
//    /** Importing data and options used for this mesh */
//    UPROPERTY(VisibleAnywhere, Instanced, Category = ImportSettings)
//        UAssetImportData* AssetImportData;
//
//    /** Information for thumbnail rendering */
//    UPROPERTY(VisibleAnywhere, Instanced, Category = Thumbnail)
//        USceneThumbnailInfo* ThumbnailInfo;
//
//    /** If the user has modified collision in any way or has custom collision imported. Used for determining if to auto generate collision on import */
//    UPROPERTY(EditAnywhere, Category = Collision)
//        bool bCustomizedCollision;
//
//    UPROPERTY(EditAnywhere, Category = Collision)
//        bool bDrawMeshCollisionWireframe;
//
//    /** The stored camera position to use as a default for the voxel database editor */
//    UPROPERTY()
//        FAssetEditorOrbitCameraPosition EditorCameraPosition;
//#endif
};
