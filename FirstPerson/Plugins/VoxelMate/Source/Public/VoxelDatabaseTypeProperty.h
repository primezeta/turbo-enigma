#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseTypeProperty.generated.h"

UCLASS(ClassGroup = VoxelMate, HideCategories=(VoxelValue, MetadataValue, TransformMapValue))
class UVoxelDatabaseTypeProperty : public UObject
{
    GENERATED_BODY()

public:
    UVoxelDatabaseTypeProperty(const FObjectInitializer& ObjectInitializer)
    {}

    UPROPERTY(Category = "VoxelValue | Bool", EditAnywhere)
        bool VoxelBool;
    UPROPERTY(Category = "VoxelValue | Float", EditAnywhere)
        float VoxelFloat;
    UPROPERTY(Category = "VoxelValue | Double", EditAnywhere)
        double VoxelDouble;
    UPROPERTY(Category = "VoxelValue | Int32", EditAnywhere)
        int32 VoxelInt32;
    UPROPERTY(Category = "VoxelValue | Int64", EditAnywhere)
        int64 VoxelInt64;
    UPROPERTY(Category = "VoxelValue | FVector2D", EditAnywhere)
        FVector2D VoxelVector2D;
    UPROPERTY(Category = "VoxelValue | FVector", EditAnywhere)
        FVector VoxelVector3D;
    UPROPERTY(Category = "VoxelValue | FVector4", EditAnywhere)
        FVector4 VoxelVector4D;
    UPROPERTY(Category = "VoxelValue | FIntVector", EditAnywhere)
        FIntVector VoxelIntVector3D;

    UPROPERTY(Category = "MetadataValue | Bool", EditAnywhere)
        bool MetadataBool;
    UPROPERTY(Category = "MetadataValue | Float", EditAnywhere)
        float MetadataFloat;
    UPROPERTY(Category = "MetadataValue | Double", EditAnywhere)
        double MetadataDouble;
    UPROPERTY(Category = "MetadataValue | Int32", EditAnywhere)
        int32 MetadataInt32;
    UPROPERTY(Category = "MetadataValue | Int64", EditAnywhere)
        int64 MetadataInt64;
    UPROPERTY(Category = "MetadataValue | FVector2D", EditAnywhere)
        FVector2D MetadataVector2D;
    UPROPERTY(Category = "MetadataValue | FVector", EditAnywhere)
        FVector MetadataVector3D;
    UPROPERTY(Category = "MetadataValue | FIntVector", EditAnywhere)
        FIntVector MetadataIntVector3D;
    UPROPERTY(Category = "MetadataValue | FString", EditAnywhere)
        FString MetadataString;

    //TODO
    //UPROPERTY(Category = "TransformMapValue | FTransformAffineMap", EditAnywhere)
    //    FTransformAffineMap MapAffine;
    //UPROPERTY(Category = "TransformMapValue | FTransformUnitaryMap", EditAnywhere)
    //    FTransformUnitaryMap MapUnitary;
    //UPROPERTY(Category = "TransformMapValue | FTransformScaleMap", EditAnywhere)
    //    FTransformScaleMap MapScale;
    //UPROPERTY(Category = "TransformMapValue | FTransformUniformScaleMap", EditAnywhere)
    //    FTransformUniformScaleMap MapUniformScale;
    //UPROPERTY(Category = "TransformMapValue | FTransformTranslateMap", EditAnywhere)
    //    FTransformTranslateMap MapTranslate;
    //UPROPERTY(Category = "TransformMapValue | FTransformScaleTranslateMap", EditAnywhere)
    //    FTransformScaleTranslateMap MapScaleTranslate;
    //UPROPERTY(Category = "TransformMapValue | FTransformUniformScaleTranslateMap", EditAnywhere)
    //    FTransformUniformScaleTranslateMap MapUniformScaleTranslate;
    //UPROPERTY(Category = "TransformMapValue | FTransformNonlinearFrustumMap", EditAnywhere)
    //    FTransformNonlinearFrustumMap MapNonlinearFrustum;
};
