#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"

const FVoxelDatabaseBoolVoxel::ValueType FVoxelDatabaseBoolVoxel::Type::ZeroValue(false);
const FVoxelDatabaseDoubleVoxel::ValueType FVoxelDatabaseDoubleVoxel::Type::ZeroValue(0.0);
const FVoxelDatabaseFloatVoxel::ValueType FVoxelDatabaseFloatVoxel::Type::ZeroValue(0.0f);
const FVoxelDatabaseInt8Voxel::ValueType FVoxelDatabaseInt8Voxel::Type::ZeroValue(0);
const FVoxelDatabaseInt16Voxel::ValueType FVoxelDatabaseInt16Voxel::Type::ZeroValue(0);
const FVoxelDatabaseInt32Voxel::ValueType FVoxelDatabaseInt32Voxel::Type::ZeroValue(0);
const FVoxelDatabaseInt64Voxel::ValueType FVoxelDatabaseInt64Voxel::Type::ZeroValue(0);
const FVoxelDatabaseUInt8Voxel::ValueType FVoxelDatabaseUInt8Voxel::Type::ZeroValue(0);
const FVoxelDatabaseUInt16Voxel::ValueType FVoxelDatabaseUInt16Voxel::Type::ZeroValue(0);
const FVoxelDatabaseUInt32Voxel::ValueType FVoxelDatabaseUInt32Voxel::Type::ZeroValue(0);
const FVoxelDatabaseUInt64Voxel::ValueType FVoxelDatabaseUInt64Voxel::Type::ZeroValue(0);
const FVoxelDatabaseVectorVoxel::ValueType FVoxelDatabaseVectorVoxel::Type::ZeroValue(FVector::ZeroVector);
const FVoxelDatabaseVector4Voxel::ValueType FVoxelDatabaseVector4Voxel::Type::ZeroValue(FVector4(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabaseVector2DVoxel::ValueType FVoxelDatabaseVector2DVoxel::Type::ZeroValue(FVector2D::ZeroVector);
const FVoxelDatabaseColorVoxel::ValueType FVoxelDatabaseColorVoxel::Type::ZeroValue(FColor(0, 0, 0, 0));
const FVoxelDatabaseLinearColorVoxel::ValueType FVoxelDatabaseLinearColorVoxel::Type::ZeroValue(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabasePackedNormalVoxel::ValueType FVoxelDatabasePackedNormalVoxel::Type::ZeroValue(FPackedNormal::ZeroNormal);
const FVoxelDatabasePackedRGB10A2NVoxel::ValueType FVoxelDatabasePackedRGB10A2NVoxel::Type::ZeroValue(FPackedRGB10A2N(511, 511, 511, 511)); //TODO confirm this works as ZeroVector
const FVoxelDatabasePackedRGBA16NVoxel::ValueType FVoxelDatabasePackedRGBA16NVoxel::Type::ZeroValue(FPackedRGBA16N(32767, 32767, 32767, 32767)); //TODO confirm this works as ZeroVector
const FVoxelDatabaseIntPointVoxel::ValueType FVoxelDatabaseIntPointVoxel::Type::ZeroValue(FVoxelDatabaseIntPointVoxel::Type::ZeroValue);
const FVoxelDatabaseIntVectorVoxel::ValueType FVoxelDatabaseIntVectorVoxel::Type::ZeroValue(FVoxelDatabaseIntVectorVoxel::Type::ZeroValue);

const FVoxelDatabaseBoolMeta::ValueType FVoxelDatabaseBoolMeta::Type::ZeroValue(false);
const FVoxelDatabaseDoubleMeta::ValueType FVoxelDatabaseDoubleMeta::Type::ZeroValue(0.0);
const FVoxelDatabaseFloatMeta::ValueType FVoxelDatabaseFloatMeta::Type::ZeroValue(0.0f);
const FVoxelDatabaseInt8Meta::ValueType FVoxelDatabaseInt8Meta::Type::ZeroValue(0);
const FVoxelDatabaseInt16Meta::ValueType FVoxelDatabaseInt16Meta::Type::ZeroValue(0);
const FVoxelDatabaseInt32Meta::ValueType FVoxelDatabaseInt32Meta::Type::ZeroValue(0);
const FVoxelDatabaseInt64Meta::ValueType FVoxelDatabaseInt64Meta::Type::ZeroValue(0);
const FVoxelDatabaseUInt8Meta::ValueType FVoxelDatabaseUInt8Meta::Type::ZeroValue(0);
const FVoxelDatabaseUInt16Meta::ValueType FVoxelDatabaseUInt16Meta::Type::ZeroValue(0);
const FVoxelDatabaseUInt32Meta::ValueType FVoxelDatabaseUInt32Meta::Type::ZeroValue(0);
const FVoxelDatabaseUInt64Meta::ValueType FVoxelDatabaseUInt64Meta::Type::ZeroValue(0);
const FVoxelDatabaseVectorMeta::ValueType FVoxelDatabaseVectorMeta::Type::ZeroValue(FVector::ZeroVector);
const FVoxelDatabaseVector4Meta::ValueType FVoxelDatabaseVector4Meta::Type::ZeroValue(FVector4(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabaseVector2DMeta::ValueType FVoxelDatabaseVector2DMeta::Type::ZeroValue(FVector2D::ZeroVector);
const FVoxelDatabaseColorMeta::ValueType FVoxelDatabaseColorMeta::Type::ZeroValue(FColor(0, 0, 0, 0));
const FVoxelDatabaseLinearColorMeta::ValueType FVoxelDatabaseLinearColorMeta::Type::ZeroValue(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabasePackedNormalMeta::ValueType FVoxelDatabasePackedNormalMeta::Type::ZeroValue(FPackedNormal::ZeroNormal);
const FVoxelDatabasePackedRGB10A2NMeta::ValueType FVoxelDatabasePackedRGB10A2NMeta::Type::ZeroValue(FPackedRGB10A2N(511, 511, 511, 511)); //TODO confirm this works as ZeroVector
const FVoxelDatabasePackedRGBA16NMeta::ValueType FVoxelDatabasePackedRGBA16NMeta::Type::ZeroValue(FPackedRGBA16N(32767, 32767, 32767, 32767)); //TODO confirm this works as ZeroVector
const FVoxelDatabaseIntPointMeta::ValueType FVoxelDatabaseIntPointMeta::Type::ZeroValue(FVoxelDatabaseIntPointMeta::Type::ZeroValue);
const FVoxelDatabaseIntVectorMeta::ValueType FVoxelDatabaseIntVectorMeta::Type::ZeroValue(FVoxelDatabaseIntVectorMeta::Type::ZeroValue);
const FVoxelDatabaseFloatRangeMeta::ValueType FVoxelDatabaseFloatRangeMeta::Type::ZeroValue(FFloatRange::Empty());
const FVoxelDatabaseInt32RangeMeta::ValueType FVoxelDatabaseInt32RangeMeta::Type::ZeroValue(FInt32Range::Empty());
const FVoxelDatabaseFloatIntervalMeta::ValueType FVoxelDatabaseFloatIntervalMeta::Type::ZeroValue(FFloatInterval(0.0f, 0.0f));
const FVoxelDatabaseInt32IntervalMeta::ValueType FVoxelDatabaseInt32IntervalMeta::Type::ZeroValue(FInt32Interval(0, 0));
const FVoxelDatabaseFloatRangeBoundMeta::ValueType FVoxelDatabaseFloatRangeBoundMeta::Type::ZeroValue(FFloatRangeBound(0.0f));
const FVoxelDatabaseInt32RangeBoundMeta::ValueType FVoxelDatabaseInt32RangeBoundMeta::Type::ZeroValue(FInt32RangeBound(0.0f));
const FVoxelDatabaseTwoVectorsMeta::ValueType FVoxelDatabaseTwoVectorsMeta::Type::ZeroValue(FTwoVectors(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabasePlaneMeta::ValueType FVoxelDatabasePlaneMeta::Type::ZeroValue = FPlane(FPlane::ZeroVector);
const FVoxelDatabaseRotatorMeta::ValueType FVoxelDatabaseRotatorMeta::Type::ZeroValue(FRotator::ZeroRotator);
const FVoxelDatabaseQuatMeta::ValueType FVoxelDatabaseQuatMeta::Type::ZeroValue = FQuat(FVoxelDatabaseRotatorMeta::Type::ZeroValue);
const FVoxelDatabaseBoxMeta::ValueType FVoxelDatabaseBoxMeta::Type::ZeroValue(FBox(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseBox2DMeta::ValueType FVoxelDatabaseBox2DMeta::Type::ZeroValue(FBox2D(FVoxelDatabaseVector2DMeta::Type::ZeroValue, FVoxelDatabaseVector2DMeta::Type::ZeroValue));
const FVoxelDatabaseBoxSphereBoundsMeta::ValueType FVoxelDatabaseBoxSphereBoundsMeta::Type::ZeroValue(FBoxSphereBounds(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, 0.0f));
const FVoxelDatabaseOrientedBoxMeta::ValueType FVoxelDatabaseOrientedBoxMeta::Type::ZeroValue = FOrientedBox();
const FVoxelDatabaseMatrixMeta::ValueType FVoxelDatabaseMatrixMeta::Type::ZeroValue(FMatrix(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurvePointFloatMeta::ValueType FVoxelDatabaseInterpCurvePointFloatMeta::Type::ZeroValue(FInterpCurvePointFloat(0.0f, 0.0f));
const FVoxelDatabaseInterpCurveFloatMeta::ValueType FVoxelDatabaseInterpCurveFloatMeta::Type::ZeroValue = FInterpCurveFloat();
const FVoxelDatabaseInterpCurvePointVector2DMeta::ValueType FVoxelDatabaseInterpCurvePointVector2DMeta::Type::ZeroValue(FInterpCurvePointVector2D(0.0f, FVoxelDatabaseVector2DMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurveVector2DMeta::ValueType FVoxelDatabaseInterpCurveVector2DMeta::Type::ZeroValue = FInterpCurveVector2D();
const FVoxelDatabaseInterpCurvePointVectorMeta::ValueType FVoxelDatabaseInterpCurvePointVectorMeta::Type::ZeroValue(FInterpCurvePointVector(0.0f, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurveVectorMeta::ValueType FVoxelDatabaseInterpCurveVectorMeta::Type::ZeroValue = FInterpCurveVector();
const FVoxelDatabaseInterpCurvePointQuatMeta::ValueType FVoxelDatabaseInterpCurvePointQuatMeta::Type::ZeroValue(FInterpCurvePointQuat(0.0f, FVoxelDatabaseQuatMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurveQuatMeta::ValueType FVoxelDatabaseInterpCurveQuatMeta::Type::ZeroValue = FInterpCurveQuat();
const FVoxelDatabaseInterpCurvePointTwoVectorsMeta::ValueType FVoxelDatabaseInterpCurvePointTwoVectorsMeta::Type::ZeroValue(FInterpCurvePointTwoVectors(0.0f, FVoxelDatabaseTwoVectorsMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurveTwoVectorsMeta::ValueType FVoxelDatabaseInterpCurveTwoVectorsMeta::Type::ZeroValue = FInterpCurveTwoVectors();
const FVoxelDatabaseInterpCurvePointLinearColorMeta::ValueType FVoxelDatabaseInterpCurvePointLinearColorMeta::Type::ZeroValue(FInterpCurvePointLinearColor(0.0f, FVoxelDatabaseLinearColorMeta::Type::ZeroValue));
const FVoxelDatabaseInterpCurveLinearColorMeta::ValueType FVoxelDatabaseInterpCurveLinearColorMeta::Type::ZeroValue = FInterpCurveLinearColor();
const FVoxelDatabaseTransformMeta::ValueType FVoxelDatabaseTransformMeta::Type::ZeroValue(FTransform(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseRandomStreamMeta::ValueType FVoxelDatabaseRandomStreamMeta::Type::ZeroValue(FRandomStream(0));
const FVoxelDatabaseDateTimeMeta::ValueType FVoxelDatabaseDateTimeMeta::Type::ZeroValue(FDateTime(0));
const FVoxelDatabaseTimespanMeta::ValueType FVoxelDatabaseTimespanMeta::Type::ZeroValue(FTimespan(0));
const FVoxelDatabaseStringAssetReferenceMeta::ValueType FVoxelDatabaseStringAssetReferenceMeta::Type::ZeroValue = FStringAssetReference();
const FVoxelDatabaseStringClassReferenceMeta::ValueType FVoxelDatabaseStringClassReferenceMeta::Type::ZeroValue = FStringClassReference();
const FVoxelDatabaseStringMeta::ValueType FVoxelDatabaseStringMeta::Type::ZeroValue(TEXT(""));
const FVoxelDatabaseNameMeta::ValueType FVoxelDatabaseNameMeta::Type::ZeroValue = FName();
const FVoxelDatabaseTextMeta::ValueType FVoxelDatabaseTextMeta::Type::ZeroValue(FText::FromString(FVoxelDatabaseStringMeta::Type::ZeroValue));
const FVoxelDatabaseAffineMapMetadataMeta::ValueType FVoxelDatabaseAffineMapMetadataMeta::Type::ZeroValue(FAffineMap(FVoxelDatabasePlaneMeta::Type::ZeroValue, FVoxelDatabasePlaneMeta::Type::ZeroValue, FVoxelDatabasePlaneMeta::Type::ZeroValue, FVoxelDatabasePlaneMeta::Type::ZeroValue));
const FVoxelDatabaseUnitaryMapMetadataMeta::ValueType FVoxelDatabaseUnitaryMapMetadataMeta::Type::ZeroValue(FUnitaryMap(FVoxelDatabaseVectorMeta::Type::ZeroValue, 0.0f));
const FVoxelDatabaseScaleMapMetadataMeta::ValueType FVoxelDatabaseScaleMapMetadataMeta::Type::ZeroValue = FScaleMap(FVoxelDatabaseVectorMeta::Type::ZeroValue);
const FVoxelDatabaseUniformScaleMapMetadataMeta::ValueType FVoxelDatabaseUniformScaleMapMetadataMeta::Type::ZeroValue(FUniformScaleMap(0.0f));
const FVoxelDatabaseTranslationMapMetadataMeta::ValueType FVoxelDatabaseTranslationMapMetadataMeta::Type::ZeroValue = FTranslationMap(FVoxelDatabaseVectorMeta::Type::ZeroValue);
const FVoxelDatabaseScaleTranslationMapMetadataMeta::ValueType FVoxelDatabaseScaleTranslationMapMetadataMeta::Type::ZeroValue(FScaleTranslationMap(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseUniformScaleTranslationMapMetadataMeta::ValueType FVoxelDatabaseUniformScaleTranslationMapMetadataMeta::Type::ZeroValue(FUniformScaleTranslationMap(0.0f, FVoxelDatabaseVectorMeta::Type::ZeroValue));
const FVoxelDatabaseNonlinearFrustumMapMetadataMeta::ValueType FVoxelDatabaseNonlinearFrustumMapMetadataMeta::Type::ZeroValue(FNonlinearFrustumMap(FVoxelDatabaseVectorMeta::Type::ZeroValue, FVoxelDatabaseVectorMeta::Type::ZeroValue, 0.0f, 0.0f));
