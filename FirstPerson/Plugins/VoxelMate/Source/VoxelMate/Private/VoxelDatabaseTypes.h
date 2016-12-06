#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"

template<typename Ue4Type>
struct TVoxelDatabaseMetadataType
{
    typedef typename Ue4Type ValueType;
    static_assert(!TIsPointer<Ue4Type>::Value, "Voxel database types cannot be a pointer type");

    TVoxelDatabaseMetadataType()
        : Value(ZeroValue.Value)
    {}

    TVoxelDatabaseMetadataType(Ue4Type&& InValue)
        : Value(InValue)
    {}

    TVoxelDatabaseMetadataType(const Ue4Type& InValue)
        : Value(InValue)
    {}

    Ue4Type Value;
    const static TVoxelDatabaseMetadataType ZeroValue;

    VOXELMATEINLINE friend OutputStreamType& operator<<(OutputStreamType& os, const TVoxelDatabaseMetadataType& InRhs)
    {
        WriteValue<Ue4Type>(os, InRhs.Value);
        return os;
    }

    VOXELMATEINLINE friend bool operator==(const TVoxelDatabaseMetadataType& InLhs, const TVoxelDatabaseMetadataType& InRhs)
    {
        return AreValuesEqual<Ue4Type>(InLhs.Value, InRhs.Value);
    }
};

template<typename Ue4Type>
struct TVoxelDatabaseVoxelType
{
    typedef typename Ue4Type ValueType;
    static_assert(!TIsPointer<ValueType>::Value, "Voxel database types cannot be a pointer type");

    TVoxelDatabaseVoxelType()
        : Value(ZeroValue.Value)
    {}

    TVoxelDatabaseVoxelType(Ue4Type&& InValue)
        : Value(InValue)
    {}

    TVoxelDatabaseVoxelType(const Ue4Type& InValue)
        : Value(InValue)
    {}

    Ue4Type Value;
    const static TVoxelDatabaseVoxelType ZeroValue;

private:
    VOXELMATEINLINE friend OutputStreamType& operator<<(OutputStreamType& os, const TVoxelDatabaseVoxelType& InRhs)
    {
        WriteValue<Ue4Type>(os, InRhs.Value);
        return os;
    }

    VOXELMATEINLINE friend bool operator==(const TVoxelDatabaseVoxelType& InLhs, const TVoxelDatabaseVoxelType& InRhs)
    {
        return AreValuesEqual<Ue4Type>(InLhs.Value, InRhs.Value);
    }

    VOXELMATEINLINE friend TVoxelDatabaseVoxelType Abs(const TVoxelDatabaseVoxelType& InType)
    {
        return AbsValue<Ue4Type>(InType.Value);
    }

    VOXELMATEINLINE friend TVoxelDatabaseVoxelType operator+(const TVoxelDatabaseVoxelType& InLhs, const float &InRhs)
    {
        return AddFloat<Ue4Type>(InLhs.Value, InRhs);
    }

    VOXELMATEINLINE friend TVoxelDatabaseVoxelType operator+(const TVoxelDatabaseVoxelType& InLhs, const TVoxelDatabaseVoxelType& InRhs)
    {
        return AddValues<Ue4Type>(InLhs.Value, InRhs.Value);
    }

    VOXELMATEINLINE friend TVoxelDatabaseVoxelType operator-(const TVoxelDatabaseVoxelType& InLhs, const TVoxelDatabaseVoxelType& InRhs)
    {
        return SubValues<Ue4Type>(InLhs.Value, InRhs.Value);
    }

    VOXELMATEINLINE friend TVoxelDatabaseVoxelType operator-(const TVoxelDatabaseVoxelType& InLhs)
    {
        return NegValue<Ue4Type>(InLhs.Value);
    }

    VOXELMATEINLINE friend bool operator<(const TVoxelDatabaseVoxelType& InLhs, const TVoxelDatabaseVoxelType& InRhs)
    {
        return IsValueLessThan<Ue4Type>(InLhs.Value, InRhs.Value);
    }

    VOXELMATEINLINE friend bool operator>(const TVoxelDatabaseVoxelType& InLhs, const TVoxelDatabaseVoxelType& InRhs)
    {
        return IsValueGreaterThan<Ue4Type>(InLhs.Value, InRhs.Value);
    }
};

typedef typename TVoxelDatabaseVoxelType<bool> FVoxelDatabaseBoolVoxel;
typedef typename TVoxelDatabaseVoxelType<double> FVoxelDatabaseDoubleVoxel;
typedef typename TVoxelDatabaseVoxelType<float> FVoxelDatabaseFloatVoxel;
typedef typename TVoxelDatabaseVoxelType<int8> FVoxelDatabaseInt8Voxel;
typedef typename TVoxelDatabaseVoxelType<int16> FVoxelDatabaseInt16Voxel;
typedef typename TVoxelDatabaseVoxelType<int32> FVoxelDatabaseInt32Voxel;
typedef typename TVoxelDatabaseVoxelType<int64> FVoxelDatabaseInt64Voxel;
typedef typename TVoxelDatabaseVoxelType<uint8> FVoxelDatabaseUInt8Voxel;
typedef typename TVoxelDatabaseVoxelType<uint16> FVoxelDatabaseUInt16Voxel;
typedef typename TVoxelDatabaseVoxelType<uint32> FVoxelDatabaseUInt32Voxel;
typedef typename TVoxelDatabaseVoxelType<uint64> FVoxelDatabaseUInt64Voxel;
typedef typename TVoxelDatabaseVoxelType<FVector> FVoxelDatabaseVectorVoxel;
typedef typename TVoxelDatabaseVoxelType<FVector4> FVoxelDatabaseVector4Voxel;
typedef typename TVoxelDatabaseVoxelType<FVector2D> FVoxelDatabaseVector2DVoxel;
typedef typename TVoxelDatabaseVoxelType<FColor> FVoxelDatabaseColorVoxel;
typedef typename TVoxelDatabaseVoxelType<FLinearColor> FVoxelDatabaseLinearColorVoxel;
typedef typename TVoxelDatabaseVoxelType<FPackedNormal> FVoxelDatabasePackedNormalVoxel;
typedef typename TVoxelDatabaseVoxelType<FPackedRGB10A2N> FVoxelDatabasePackedRGB10A2NVoxel;
typedef typename TVoxelDatabaseVoxelType<FPackedRGBA16N> FVoxelDatabasePackedRGBA16NVoxel;
typedef typename TVoxelDatabaseVoxelType<FIntPoint> FVoxelDatabaseIntPointVoxel;
typedef typename TVoxelDatabaseVoxelType<FIntVector> FVoxelDatabaseIntVectorVoxel;

typedef typename TVoxelDatabaseMetadataType<bool> FVoxelDatabaseBoolMeta;
typedef typename TVoxelDatabaseMetadataType<double> FVoxelDatabaseDoubleMeta;
typedef typename TVoxelDatabaseMetadataType<float> FVoxelDatabaseFloatMeta;
typedef typename TVoxelDatabaseMetadataType<int8> FVoxelDatabaseInt8Meta;
typedef typename TVoxelDatabaseMetadataType<int16> FVoxelDatabaseInt16Meta;
typedef typename TVoxelDatabaseMetadataType<int32> FVoxelDatabaseInt32Meta;
typedef typename TVoxelDatabaseMetadataType<int64> FVoxelDatabaseInt64Meta;
typedef typename TVoxelDatabaseMetadataType<uint8> FVoxelDatabaseUInt8Meta;
typedef typename TVoxelDatabaseMetadataType<uint16> FVoxelDatabaseUInt16Meta;
typedef typename TVoxelDatabaseMetadataType<uint32> FVoxelDatabaseUInt32Meta;
typedef typename TVoxelDatabaseMetadataType<uint64> FVoxelDatabaseUInt64Meta;
typedef typename TVoxelDatabaseMetadataType<FVector> FVoxelDatabaseVectorMeta;
typedef typename TVoxelDatabaseMetadataType<FVector4> FVoxelDatabaseVector4Meta;
typedef typename TVoxelDatabaseMetadataType<FVector2D> FVoxelDatabaseVector2DMeta;
typedef typename TVoxelDatabaseMetadataType<FColor> FVoxelDatabaseColorMeta;
typedef typename TVoxelDatabaseMetadataType<FLinearColor> FVoxelDatabaseLinearColorMeta;
typedef typename TVoxelDatabaseMetadataType<FPackedNormal> FVoxelDatabasePackedNormalMeta;
typedef typename TVoxelDatabaseMetadataType<FPackedRGB10A2N> FVoxelDatabasePackedRGB10A2NMeta;
typedef typename TVoxelDatabaseMetadataType<FPackedRGBA16N> FVoxelDatabasePackedRGBA16NMeta;
typedef typename TVoxelDatabaseMetadataType<FIntPoint> FVoxelDatabaseIntPointMeta;
typedef typename TVoxelDatabaseMetadataType<FIntVector> FVoxelDatabaseIntVectorMeta;
typedef typename TVoxelDatabaseMetadataType<FFloatRange> FVoxelDatabaseFloatRangeMeta;
typedef typename TVoxelDatabaseMetadataType<FInt32Range> FVoxelDatabaseInt32RangeMeta;
typedef typename TVoxelDatabaseMetadataType<FFloatInterval> FVoxelDatabaseFloatIntervalMeta;
typedef typename TVoxelDatabaseMetadataType<FInt32Interval> FVoxelDatabaseInt32IntervalMeta;
typedef typename TVoxelDatabaseMetadataType<FFloatRangeBound> FVoxelDatabaseFloatRangeBoundMeta;
typedef typename TVoxelDatabaseMetadataType<FInt32RangeBound> FVoxelDatabaseInt32RangeBoundMeta;
typedef typename TVoxelDatabaseMetadataType<FTwoVectors> FVoxelDatabaseTwoVectorsMeta;
typedef typename TVoxelDatabaseMetadataType<FPlane> FVoxelDatabasePlaneMeta;
typedef typename TVoxelDatabaseMetadataType<FRotator> FVoxelDatabaseRotatorMeta;
typedef typename TVoxelDatabaseMetadataType<FQuat> FVoxelDatabaseQuatMeta;
typedef typename TVoxelDatabaseMetadataType<FBox> FVoxelDatabaseBoxMeta;
typedef typename TVoxelDatabaseMetadataType<FBox2D> FVoxelDatabaseBox2DMeta;
typedef typename TVoxelDatabaseMetadataType<FBoxSphereBounds> FVoxelDatabaseBoxSphereBoundsMeta;
typedef typename TVoxelDatabaseMetadataType<FOrientedBox> FVoxelDatabaseOrientedBoxMeta;
typedef typename TVoxelDatabaseMetadataType<FMatrix> FVoxelDatabaseMatrixMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointFloat> FVoxelDatabaseInterpCurvePointFloatMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveFloat> FVoxelDatabaseInterpCurveFloatMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointVector2D> FVoxelDatabaseInterpCurvePointVector2DMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveVector2D> FVoxelDatabaseInterpCurveVector2DMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointVector> FVoxelDatabaseInterpCurvePointVectorMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveVector> FVoxelDatabaseInterpCurveVectorMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointQuat> FVoxelDatabaseInterpCurvePointQuatMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveQuat> FVoxelDatabaseInterpCurveQuatMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointTwoVectors> FVoxelDatabaseInterpCurvePointTwoVectorsMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveTwoVectors> FVoxelDatabaseInterpCurveTwoVectorsMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurvePointLinearColor> FVoxelDatabaseInterpCurvePointLinearColorMeta;
typedef typename TVoxelDatabaseMetadataType<FInterpCurveLinearColor> FVoxelDatabaseInterpCurveLinearColorMeta;
typedef typename TVoxelDatabaseMetadataType<FTransform> FVoxelDatabaseTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FRandomStream> FVoxelDatabaseRandomStreamMeta;
typedef typename TVoxelDatabaseMetadataType<FDateTime> FVoxelDatabaseDateTimeMeta;
typedef typename TVoxelDatabaseMetadataType<FTimespan> FVoxelDatabaseTimespanMeta;
typedef typename TVoxelDatabaseMetadataType<FStringAssetReference> FVoxelDatabaseStringAssetReferenceMeta;
typedef typename TVoxelDatabaseMetadataType<FStringClassReference> FVoxelDatabaseStringClassReferenceMeta;
typedef typename TVoxelDatabaseMetadataType<FString> FVoxelDatabaseStringMeta;
typedef typename TVoxelDatabaseMetadataType<FName> FVoxelDatabaseNameMeta;
typedef typename TVoxelDatabaseMetadataType<FText> FVoxelDatabaseTextMeta;
typedef typename TVoxelDatabaseMetadataType<FAffineCoordinateTransform> FVoxelDatabaseAffineCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FUnitaryCoordinateTransform> FVoxelDatabaseUnitaryCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FScaleCoordinateTransform> FVoxelDatabaseScaleCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FUniformScaleCoordinateTransform> FVoxelDatabaseUniformScaleCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FTranslationCoordinateTransform> FVoxelDatabaseTranslationCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FScaleTranslationCoordinateTransform> FVoxelDatabaseScaleTranslationCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FUniformScaleTranslationCoordinateTransform> FVoxelDatabaseUniformScaleTranslationCoordinateTransformMeta;
typedef typename TVoxelDatabaseMetadataType<FNonlinearFrustumCoordinateTransform> FVoxelDatabaseNonlinearFrustumCoordinateTransformMeta;
