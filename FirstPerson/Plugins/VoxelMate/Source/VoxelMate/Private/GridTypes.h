#pragma once
#include "VoxelDatabaseTypes.h"
#include <openvdb/openvdb.h>

template<> VOXELMATEINLINE FVoxelDatabaseBoolVoxel openvdb::zeroVal<FVoxelDatabaseBoolVoxel>()
{
    return FVoxelDatabaseBoolVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseDoubleVoxel openvdb::zeroVal<FVoxelDatabaseDoubleVoxel>()
{
    return FVoxelDatabaseDoubleVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseFloatVoxel openvdb::zeroVal<FVoxelDatabaseFloatVoxel>()
{
    return FVoxelDatabaseFloatVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt8Voxel openvdb::zeroVal<FVoxelDatabaseInt8Voxel>()
{
    return FVoxelDatabaseInt8Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt16Voxel openvdb::zeroVal<FVoxelDatabaseInt16Voxel>()
{
    return FVoxelDatabaseInt16Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt32Voxel openvdb::zeroVal<FVoxelDatabaseInt32Voxel>()
{
    return FVoxelDatabaseInt32Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt64Voxel openvdb::zeroVal<FVoxelDatabaseInt64Voxel>()
{
    return FVoxelDatabaseInt64Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt8Voxel openvdb::zeroVal<FVoxelDatabaseUInt8Voxel>()
{
    return FVoxelDatabaseUInt8Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt16Voxel openvdb::zeroVal<FVoxelDatabaseUInt16Voxel>()
{
    return FVoxelDatabaseUInt16Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt32Voxel openvdb::zeroVal<FVoxelDatabaseUInt32Voxel>()
{
    return FVoxelDatabaseUInt32Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt64Voxel openvdb::zeroVal<FVoxelDatabaseUInt64Voxel>()
{
    return FVoxelDatabaseUInt64Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVector2DVoxel openvdb::zeroVal<FVoxelDatabaseVector2DVoxel>()
{
    return FVoxelDatabaseVector2DVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVectorVoxel openvdb::zeroVal<FVoxelDatabaseVectorVoxel>()
{
    return FVoxelDatabaseVectorVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVector4Voxel openvdb::zeroVal<FVoxelDatabaseVector4Voxel>()
{
    return FVoxelDatabaseVector4Voxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseColorVoxel openvdb::zeroVal<FVoxelDatabaseColorVoxel>()
{
    return FVoxelDatabaseColorVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseLinearColorVoxel openvdb::zeroVal<FVoxelDatabaseLinearColorVoxel>()
{
    return FVoxelDatabaseLinearColorVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedNormalVoxel openvdb::zeroVal<FVoxelDatabasePackedNormalVoxel>()
{
    return FVoxelDatabasePackedNormalVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedRGB10A2NVoxel openvdb::zeroVal<FVoxelDatabasePackedRGB10A2NVoxel>()
{
    return FVoxelDatabasePackedRGB10A2NVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedRGBA16NVoxel openvdb::zeroVal<FVoxelDatabasePackedRGBA16NVoxel>()
{
    return FVoxelDatabasePackedRGBA16NVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseIntPointVoxel openvdb::zeroVal<FVoxelDatabaseIntPointVoxel>()
{
    return FVoxelDatabaseIntPointVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseIntVectorVoxel openvdb::zeroVal<FVoxelDatabaseIntVectorVoxel>()
{
    return FVoxelDatabaseIntVectorVoxel::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseBoolMeta openvdb::zeroVal<FVoxelDatabaseBoolMeta>()
{
    return FVoxelDatabaseBoolMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseDoubleMeta openvdb::zeroVal<FVoxelDatabaseDoubleMeta>()
{
    return FVoxelDatabaseDoubleMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseFloatMeta openvdb::zeroVal<FVoxelDatabaseFloatMeta>()
{
    return FVoxelDatabaseFloatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt8Meta openvdb::zeroVal<FVoxelDatabaseInt8Meta>()
{
    return FVoxelDatabaseInt8Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt16Meta openvdb::zeroVal<FVoxelDatabaseInt16Meta>()
{
    return FVoxelDatabaseInt16Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt32Meta openvdb::zeroVal<FVoxelDatabaseInt32Meta>()
{
    return FVoxelDatabaseInt32Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt64Meta openvdb::zeroVal<FVoxelDatabaseInt64Meta>()
{
    return FVoxelDatabaseInt64Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt8Meta openvdb::zeroVal<FVoxelDatabaseUInt8Meta>()
{
    return FVoxelDatabaseUInt8Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt16Meta openvdb::zeroVal<FVoxelDatabaseUInt16Meta>()
{
    return FVoxelDatabaseUInt16Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt32Meta openvdb::zeroVal<FVoxelDatabaseUInt32Meta>()
{
    return FVoxelDatabaseUInt32Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUInt64Meta openvdb::zeroVal<FVoxelDatabaseUInt64Meta>()
{
    return FVoxelDatabaseUInt64Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVector2DMeta openvdb::zeroVal<FVoxelDatabaseVector2DMeta>()
{
    return FVoxelDatabaseVector2DMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVectorMeta openvdb::zeroVal<FVoxelDatabaseVectorMeta>()
{
    return FVoxelDatabaseVectorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseVector4Meta openvdb::zeroVal<FVoxelDatabaseVector4Meta>()
{
    return FVoxelDatabaseVector4Meta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseColorMeta openvdb::zeroVal<FVoxelDatabaseColorMeta>()
{
    return FVoxelDatabaseColorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseLinearColorMeta openvdb::zeroVal<FVoxelDatabaseLinearColorMeta>()
{
    return FVoxelDatabaseLinearColorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedNormalMeta openvdb::zeroVal<FVoxelDatabasePackedNormalMeta>()
{
    return FVoxelDatabasePackedNormalMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedRGB10A2NMeta openvdb::zeroVal<FVoxelDatabasePackedRGB10A2NMeta>()
{
    return FVoxelDatabasePackedRGB10A2NMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePackedRGBA16NMeta openvdb::zeroVal<FVoxelDatabasePackedRGBA16NMeta>()
{
    return FVoxelDatabasePackedRGBA16NMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseIntPointMeta openvdb::zeroVal<FVoxelDatabaseIntPointMeta>()
{
    return FVoxelDatabaseIntPointMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseIntVectorMeta openvdb::zeroVal<FVoxelDatabaseIntVectorMeta>()
{
    return FVoxelDatabaseIntVectorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseFloatRangeMeta openvdb::zeroVal<FVoxelDatabaseFloatRangeMeta>()
{
    return FVoxelDatabaseFloatRangeMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt32RangeMeta openvdb::zeroVal<FVoxelDatabaseInt32RangeMeta>()
{
    return FVoxelDatabaseInt32RangeMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseFloatIntervalMeta openvdb::zeroVal<FVoxelDatabaseFloatIntervalMeta>()
{
    return FVoxelDatabaseFloatIntervalMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt32IntervalMeta openvdb::zeroVal<FVoxelDatabaseInt32IntervalMeta>()
{
    return FVoxelDatabaseInt32IntervalMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseFloatRangeBoundMeta openvdb::zeroVal<FVoxelDatabaseFloatRangeBoundMeta>()
{
    return FVoxelDatabaseFloatRangeBoundMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInt32RangeBoundMeta openvdb::zeroVal<FVoxelDatabaseInt32RangeBoundMeta>()
{
    return FVoxelDatabaseInt32RangeBoundMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseTwoVectorsMeta openvdb::zeroVal<FVoxelDatabaseTwoVectorsMeta>()
{
    return FVoxelDatabaseTwoVectorsMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabasePlaneMeta openvdb::zeroVal<FVoxelDatabasePlaneMeta>()
{
    return FVoxelDatabasePlaneMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseRotatorMeta openvdb::zeroVal<FVoxelDatabaseRotatorMeta>()
{
    return FVoxelDatabaseRotatorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseQuatMeta openvdb::zeroVal<FVoxelDatabaseQuatMeta>()
{
    return FVoxelDatabaseQuatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseBoxMeta openvdb::zeroVal<FVoxelDatabaseBoxMeta>()
{
    return FVoxelDatabaseBoxMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseBox2DMeta openvdb::zeroVal<FVoxelDatabaseBox2DMeta>()
{
    return FVoxelDatabaseBox2DMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseBoxSphereBoundsMeta openvdb::zeroVal<FVoxelDatabaseBoxSphereBoundsMeta>()
{
    return FVoxelDatabaseBoxSphereBoundsMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseOrientedBoxMeta openvdb::zeroVal<FVoxelDatabaseOrientedBoxMeta>()
{
    return FVoxelDatabaseOrientedBoxMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseMatrixMeta openvdb::zeroVal<FVoxelDatabaseMatrixMeta>()
{
    return FVoxelDatabaseMatrixMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointFloatMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointFloatMeta>()
{
    return FVoxelDatabaseInterpCurvePointFloatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveFloatMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveFloatMeta>()
{
    return FVoxelDatabaseInterpCurveFloatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointVector2DMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointVector2DMeta>()
{
    return FVoxelDatabaseInterpCurvePointVector2DMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveVector2DMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveVector2DMeta>()
{
    return FVoxelDatabaseInterpCurveVector2DMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointVectorMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointVectorMeta>()
{
    return FVoxelDatabaseInterpCurvePointVectorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveVectorMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveVectorMeta>()
{
    return FVoxelDatabaseInterpCurveVectorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointQuatMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointQuatMeta>()
{
    return FVoxelDatabaseInterpCurvePointQuatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveQuatMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveQuatMeta>()
{
    return FVoxelDatabaseInterpCurveQuatMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointTwoVectorsMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointTwoVectorsMeta>()
{
    return FVoxelDatabaseInterpCurvePointTwoVectorsMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveTwoVectorsMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveTwoVectorsMeta>()
{
    return FVoxelDatabaseInterpCurveTwoVectorsMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurvePointLinearColorMeta openvdb::zeroVal<FVoxelDatabaseInterpCurvePointLinearColorMeta>()
{
    return FVoxelDatabaseInterpCurvePointLinearColorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseInterpCurveLinearColorMeta openvdb::zeroVal<FVoxelDatabaseInterpCurveLinearColorMeta>()
{
    return FVoxelDatabaseInterpCurveLinearColorMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseTransformMeta openvdb::zeroVal<FVoxelDatabaseTransformMeta>()
{
    return FVoxelDatabaseTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseRandomStreamMeta openvdb::zeroVal<FVoxelDatabaseRandomStreamMeta>()
{
    return FVoxelDatabaseRandomStreamMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseDateTimeMeta openvdb::zeroVal<FVoxelDatabaseDateTimeMeta>()
{
    return FVoxelDatabaseDateTimeMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseTimespanMeta openvdb::zeroVal<FVoxelDatabaseTimespanMeta>()
{
    return FVoxelDatabaseTimespanMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseStringAssetReferenceMeta openvdb::zeroVal<FVoxelDatabaseStringAssetReferenceMeta>()
{
    return FVoxelDatabaseStringAssetReferenceMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseStringClassReferenceMeta openvdb::zeroVal<FVoxelDatabaseStringClassReferenceMeta>()
{
    return FVoxelDatabaseStringClassReferenceMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseStringMeta openvdb::zeroVal<FVoxelDatabaseStringMeta>()
{
    return FVoxelDatabaseStringMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseNameMeta openvdb::zeroVal<FVoxelDatabaseNameMeta>()
{
    return FVoxelDatabaseNameMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseTextMeta openvdb::zeroVal<FVoxelDatabaseTextMeta>()
{
    return FVoxelDatabaseTextMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseAffineCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseAffineCoordinateTransformMeta>()
{
    return FVoxelDatabaseAffineCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUnitaryCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseUnitaryCoordinateTransformMeta>()
{
    return FVoxelDatabaseUnitaryCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseScaleCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseScaleCoordinateTransformMeta>()
{
    return FVoxelDatabaseScaleCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUniformScaleCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseUniformScaleCoordinateTransformMeta>()
{
    return FVoxelDatabaseUniformScaleCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseTranslationCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseTranslationCoordinateTransformMeta>()
{
    return FVoxelDatabaseTranslationCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseScaleTranslationCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseScaleTranslationCoordinateTransformMeta>()
{
    return FVoxelDatabaseScaleTranslationCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseUniformScaleTranslationCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseUniformScaleTranslationCoordinateTransformMeta>()
{
    return FVoxelDatabaseUniformScaleTranslationCoordinateTransformMeta::ZeroValue;
}

template<> VOXELMATEINLINE FVoxelDatabaseNonlinearFrustumCoordinateTransformMeta openvdb::zeroVal<FVoxelDatabaseNonlinearFrustumCoordinateTransformMeta>()
{
    return FVoxelDatabaseNonlinearFrustumCoordinateTransformMeta::ZeroValue;
}

//template<> struct openvdb::math::Tolerance<FVoxelDatabaseBoolVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseBoolVoxel value() { return openvdb::math::Tolerance<bool>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseDoubleVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseDoubleVoxel value() { return openvdb::math::Tolerance<double>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseFloatVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseFloatVoxel value() { return openvdb::math::Tolerance<float>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseInt8Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt8Voxel value() { return openvdb::math::Tolerance<int8>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseInt16Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt16Voxel value() { return openvdb::math::Tolerance<int16>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseInt32Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt32Voxel value() { return openvdb::math::Tolerance<int32>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseInt64Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt64Voxel value() { return openvdb::math::Tolerance<int64>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseUInt8Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt8Voxel value() { return openvdb::math::Tolerance<uint8>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseUInt16Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt16Voxel value() { return openvdb::math::Tolerance<uint16>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseUInt32Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt32Voxel value() { return openvdb::math::Tolerance<uint32>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseUInt64Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt64Voxel value() { return openvdb::math::Tolerance<uint64>::value(); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseVectorVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVectorVoxel value() { return FVector(openvdb::math::Tolerance<float>::value()); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseVector4Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVector4Voxel value() { return FVector4(openvdb::math::Tolerance<float>::value()); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseVector2DVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVector2DVoxel value() { return FVector2D(openvdb::math::Tolerance<float>::value(), openvdb::math::Tolerance<float>::value()); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabaseLinearColorVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseLinearColorVoxel value() { return FLinearColor(FVector4(openvdb::math::Tolerance<float>::value())); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedNormalVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedNormalVoxel value() { return FPackedNormal(FVector4(openvdb::math::Tolerance<float>::value())); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedRGB10A2NVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedRGB10A2NVoxel value() { return FPackedRGB10A2N(FVector4(openvdb::math::Tolerance<float>::value())); }
//};
//
//template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedRGBA16NVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedRGBA16NVoxel value() { return FPackedRGBA16N(FVector4(openvdb::math::Tolerance<float>::value())); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseBoolVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseBoolVoxel value() { return openvdb::math::Delta<bool>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseDoubleVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseDoubleVoxel value() { return openvdb::math::Delta<double>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseFloatVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseFloatVoxel value() { return openvdb::math::Delta<float>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseInt8Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt8Voxel value() { return openvdb::math::Delta<int8>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseInt16Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt16Voxel value() { return openvdb::math::Delta<int16>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseInt32Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt32Voxel value() { return openvdb::math::Delta<int32>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseInt64Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseInt64Voxel value() { return openvdb::math::Delta<int64>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseUInt8Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt8Voxel value() { return openvdb::math::Delta<uint8>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseUInt16Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt16Voxel value() { return openvdb::math::Delta<uint16>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseUInt32Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt32Voxel value() { return openvdb::math::Delta<uint32>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseUInt64Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseUInt64Voxel value() { return openvdb::math::Delta<uint64>::value(); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseVectorVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVectorVoxel value() { return FVector(openvdb::math::Delta<float>::value()); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseVector4Voxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVector4Voxel value() { return FVector4(openvdb::math::Delta<float>::value()); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseVector2DVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseVector2DVoxel value() { return FVector2D(openvdb::math::Delta<float>::value(), openvdb::math::Delta<float>::value()); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabaseLinearColorVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabaseLinearColorVoxel value() { return FLinearColor(FVector4(openvdb::math::Delta<float>::value())); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabasePackedNormalVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedNormalVoxel value() { return FPackedNormal(FVector4(openvdb::math::Delta<float>::value())); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabasePackedRGB10A2NVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedRGB10A2NVoxel value() { return FPackedRGB10A2N(FVector4(openvdb::math::Delta<float>::value())); }
//};
//
//template<> struct openvdb::math::Delta<FVoxelDatabasePackedRGBA16NVoxel>()
//{
//    VOXELMATEINLINE static FVoxelDatabasePackedRGBA16NVoxel value() { return FPackedRGBA16N(FVector4(openvdb::math::Delta<float>::value())); }
//};
