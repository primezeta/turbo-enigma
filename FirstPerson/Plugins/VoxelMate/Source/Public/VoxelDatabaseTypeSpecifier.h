#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"
#include "TypedProperty.h"
#include "VoxelDatabaseTypeSpecifier.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, EditInlineNew, HideCategories=(VoxelValue, MetadataValue, TransformMapValue))
class UVoxelDatabaseTypeSpecifier : public UObject
{
    GENERATED_BODY()

public:
    UVoxelDatabaseTypeSpecifier(const FObjectInitializer& ObjectInitializer)
        : Type((EVoxelDatabaseType)0), Name(TEXT("")), NameCount(-1)
    {
        Type = (EVoxelDatabaseType)0;
        Name = TEXT("");
        NameCount = 0;
    }

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
        EVoxelDatabaseType Type; //TODO better property names

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
        FString Name;

    UPROPERTY()
        int32 NameCount;

    UPROPERTY(Category = "VoxelValue|Bool", EditAnywhere)
        bool VoxelBool;
    UPROPERTY(Category = "VoxelValue|Float", EditAnywhere)
        float VoxelFloat;
    UPROPERTY(Category = "VoxelValue|Double", EditAnywhere)
        double VoxelDouble;
    UPROPERTY(Category = "VoxelValue|Int32", EditAnywhere)
        int32 VoxelInt32;
    UPROPERTY(Category = "VoxelValue|Int64", EditAnywhere)
        int64 VoxelInt64;
    UPROPERTY(Category = "VoxelValue|FVector2D", EditAnywhere)
        FVector2D VoxelVector2D;
    UPROPERTY(Category = "VoxelValue|FVector", EditAnywhere)
        FVector VoxelVector3D;
    UPROPERTY(Category = "VoxelValue|FVector4", EditAnywhere)
        FVector4 VoxelVector4D;
    UPROPERTY(Category = "VoxelValue|FIntVector", EditAnywhere)
        FIntVector VoxelIntVector3D;

    UPROPERTY(Category = "MetadataValue|Bool", EditAnywhere)
        bool MetadataBool;
    UPROPERTY(Category = "MetadataValue|Float", EditAnywhere)
        float MetadataFloat;
    UPROPERTY(Category = "MetadataValue|Double", EditAnywhere)
        double MetadataDouble;
    UPROPERTY(Category = "MetadataValue|Int32", EditAnywhere)
        int32 MetadataInt32;
    UPROPERTY(Category = "MetadataValue|Int64", EditAnywhere)
        int64 MetadataInt64;
    UPROPERTY(Category = "MetadataValue|FVector2D", EditAnywhere)
        FVector2D MetadataVector2D;
    UPROPERTY(Category = "MetadataValue|FVector", EditAnywhere)
        FVector MetadataVector3D;
    UPROPERTY(Category = "MetadataValue|FIntVector", EditAnywhere)
        FIntVector MetadataIntVector3D;
    UPROPERTY(Category = "MetadataValue|FString", EditAnywhere)
        FString MetadataString;

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

    UFUNCTION()
        FString GetTypeName() const
        {
            const FString TypeName = EnumValueToString<EVoxelDatabaseType>(Type);
            return TypeName;
        }

#if WITH_EDITOR
    TArray<FName> Names;
    TArray<FString> Strs;
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        Super::PostEditChangeProperty(PropertyChangedEvent);
        UProperty* Property = PropertyChangedEvent.Property;
        const FString& PropertyName = Property->GetName();
        if (PropertyName == TEXT("Type"))
        {
            TMap<FName, FString>* Meta = UMetaData::GetMapForObject(GetClass());
            for (auto i = Meta->CreateConstIterator(); i; ++i)
            {
                Names.Add(i.Key());
                Strs.Add(i.Value());
            }
            switch (Type)
            {
            case EVoxelDatabaseType::BoolType:
                break;
            case EVoxelDatabaseType::FloatType:
                break;
            case EVoxelDatabaseType::DoubleType:
                break;
            case EVoxelDatabaseType::Int32Type:
                break;
            case EVoxelDatabaseType::Int64Type:
                break;
            case EVoxelDatabaseType::FloatVector2DType:
                break;
            case EVoxelDatabaseType::FloatVector3DType:
                break;
            case EVoxelDatabaseType::FloatVector4DType:
                break;
            case EVoxelDatabaseType::Int32Vector3DType:
                break;
            case EVoxelDatabaseType::StringType:
                break;
            default:
                break;
            }
        }
    }
#endif // WITH_EDITOR
};
