#pragma once
#include "EngineMinimal.h"
#include "TypedProperty.generated.h"

USTRUCT(BlueprintType)
struct FTypedProperty
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
        EVoxelDatabaseType Type;
    UPROPERTY(Category = "VoxelDatabaseValue|Bool", EditAnywhere)
        bool BoolValue;
    UPROPERTY(Category = "VoxelDatabaseValue|Float", EditAnywhere)
        float FloatValue;
    UPROPERTY(Category = "VoxelDatabaseValue|Double", EditAnywhere)
        double DoubleValue;
    UPROPERTY(Category = "VoxelDatabaseValue|Int32", EditAnywhere)
        int32 Int32Value;
    UPROPERTY(Category = "VoxelDatabaseValue|Int64", EditAnywhere)
        int64 Int64Value;
    UPROPERTY(Category = "VoxelDatabaseValue|Vector2D", EditAnywhere)
        FVector2D Vector2DValue;
    UPROPERTY(Category = "VoxelDatabaseValue|Vector", EditAnywhere)
        FVector VectorValue;
    UPROPERTY(Category = "VoxelDatabaseValue|Vector4", EditAnywhere)
        FVector4 Vector4Value;
    UPROPERTY(Category = "VoxelDatabaseValue|IntVector", EditAnywhere)
        FIntVector IntVectorValue;
    UPROPERTY(Category = "VoxelDatabaseValue|String", EditAnywhere)
        FString StringValue;

    //TODO
    //UPROPERTY(Category = "TransformMapValue|FTransformAffineMap", EditAnywhere)
    //    FTransformAffineMap MapAffine;
    //UPROPERTY(Category = "TransformMapValue|FTransformUnitaryMap", EditAnywhere)
    //    FTransformUnitaryMap MapUnitary;
    //UPROPERTY(Category = "TransformMapValue|FTransformScaleMap", EditAnywhere)
    //    FTransformScaleMap MapScale;
    //UPROPERTY(Category = "TransformMapValue|FTransformUniformScaleMap", EditAnywhere)
    //    FTransformUniformScaleMap MapUniformScale;
    //UPROPERTY(Category = "TransformMapValue|FTransformTranslateMap", EditAnywhere)
    //    FTransformTranslateMap MapTranslate;
    //UPROPERTY(Category = "TransformMapValue|FTransformScaleTranslateMap", EditAnywhere)
    //    FTransformScaleTranslateMap MapScaleTranslate;
    //UPROPERTY(Category = "TransformMapValue|FTransformUniformScaleTranslateMap", EditAnywhere)
    //    FTransformUniformScaleTranslateMap MapUniformScaleTranslate;
    //UPROPERTY(Category = "TransformMapValue|FTransformNonlinearFrustumMap", EditAnywhere)
    //    FTransformNonlinearFrustumMap MapNonlinearFrustum;
};
