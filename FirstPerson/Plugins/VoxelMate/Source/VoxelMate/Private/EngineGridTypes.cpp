#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"

const FVoxelDatabaseBool FVoxelDatabaseBool::ZeroValue(false);
const FVoxelDatabaseDouble FVoxelDatabaseDouble::ZeroValue(0.0);
const FVoxelDatabaseFloat FVoxelDatabaseFloat::ZeroValue(0.0f);
const FVoxelDatabaseInt8 FVoxelDatabaseInt8::ZeroValue(0);
const FVoxelDatabaseInt16 FVoxelDatabaseInt16::ZeroValue(0);
const FVoxelDatabaseInt32 FVoxelDatabaseInt32::ZeroValue(0);
const FVoxelDatabaseInt64 FVoxelDatabaseInt64::ZeroValue(0);
const FVoxelDatabaseUInt8 FVoxelDatabaseUInt8::ZeroValue(0);
const FVoxelDatabaseUInt16 FVoxelDatabaseUInt16::ZeroValue(0);
const FVoxelDatabaseUInt32 FVoxelDatabaseUInt32::ZeroValue(0);
const FVoxelDatabaseUInt64 FVoxelDatabaseUInt64::ZeroValue(0);
const FVoxelDatabaseVector FVoxelDatabaseVector::ZeroValue(FVector::ZeroVector);
const FVoxelDatabaseVector4 FVoxelDatabaseVector4::ZeroValue(FVector4(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabaseVector2D FVoxelDatabaseVector2D::ZeroValue(FVector2D::ZeroVector);
const FVoxelDatabaseColor FVoxelDatabaseColor::ZeroValue(FColor(0, 0, 0, 0));
const FVoxelDatabaseLinearColor FVoxelDatabaseLinearColor::ZeroValue(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
const FVoxelDatabasePackedNormal FVoxelDatabasePackedNormal::ZeroValue(FPackedNormal::ZeroNormal);
const FVoxelDatabasePackedRGB10A2N FVoxelDatabasePackedRGB10A2N::ZeroValue(FPackedRGB10A2N(511, 511, 511, 511)); //TODO confirm this works as ZeroVector
const FVoxelDatabasePackedRGBA16N FVoxelDatabasePackedRGBA16N::ZeroValue(FPackedRGBA16N(32767, 32767, 32767, 32767)); //TODO confirm this works as ZeroVector
const FVoxelDatabaseIntPoint FVoxelDatabaseIntPoint::ZeroValue(FIntPoint::ZeroValue);
const FVoxelDatabaseIntVector FVoxelDatabaseIntVector::ZeroValue(FIntVector::ZeroValue);
const FVoxelDatabaseFloatRange FVoxelDatabaseFloatRange::ZeroValue(FFloatRange::Empty());
const FVoxelDatabaseInt32Range FVoxelDatabaseInt32Range::ZeroValue(FInt32Range::Empty());
const FVoxelDatabaseFloatInterval FVoxelDatabaseFloatInterval::ZeroValue(FFloatInterval(0.0f, 0.0f));
const FVoxelDatabaseInt32Interval FVoxelDatabaseInt32Interval::ZeroValue(FInt32Interval(0, 0));
const FVoxelDatabaseFloatRangeBound FVoxelDatabaseFloatRangeBound::ZeroValue(FFloatRangeBound(0.0f));
const FVoxelDatabaseInt32RangeBound FVoxelDatabaseInt32RangeBound::ZeroValue(FInt32RangeBound(0.0f));
const FVoxelDatabaseTwoVectors FVoxelDatabaseTwoVectors::ZeroValue(FTwoVectors(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabasePlane FVoxelDatabasePlane::ZeroValue = FPlane(FPlane::ZeroVector);
const FVoxelDatabaseRotator FVoxelDatabaseRotator::ZeroValue(FRotator::ZeroRotator);
const FVoxelDatabaseQuat FVoxelDatabaseQuat::ZeroValue(FQuat(FVoxelDatabaseRotator::ZeroValue.Value));
const FVoxelDatabaseBox FVoxelDatabaseBox::ZeroValue(FBox(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseBox2D FVoxelDatabaseBox2D::ZeroValue(FBox2D(FVoxelDatabaseVector2D::ZeroValue.Value, FVoxelDatabaseVector2D::ZeroValue.Value));
const FVoxelDatabaseBoxSphereBounds FVoxelDatabaseBoxSphereBounds::ZeroValue(FBoxSphereBounds(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, 0.0f));
const FVoxelDatabaseOrientedBox FVoxelDatabaseOrientedBox::ZeroValue = FOrientedBox();
const FVoxelDatabaseMatrix FVoxelDatabaseMatrix::ZeroValue(FMatrix(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseInterpCurvePointFloat FVoxelDatabaseInterpCurvePointFloat::ZeroValue(FInterpCurvePointFloat(0.0f, 0.0f));
const FVoxelDatabaseInterpCurveFloat FVoxelDatabaseInterpCurveFloat::ZeroValue = FInterpCurveFloat();
const FVoxelDatabaseInterpCurvePointVector2D FVoxelDatabaseInterpCurvePointVector2D::ZeroValue(FInterpCurvePointVector2D(0.0f, FVoxelDatabaseVector2D::ZeroValue.Value));
const FVoxelDatabaseInterpCurveVector2D FVoxelDatabaseInterpCurveVector2D::ZeroValue = FInterpCurveVector2D();
const FVoxelDatabaseInterpCurvePointVector FVoxelDatabaseInterpCurvePointVector::ZeroValue(FInterpCurvePointVector(0.0f, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseInterpCurveVector FVoxelDatabaseInterpCurveVector::ZeroValue = FInterpCurveVector();
const FVoxelDatabaseInterpCurvePointQuat FVoxelDatabaseInterpCurvePointQuat::ZeroValue(FInterpCurvePointQuat(0.0f, FVoxelDatabaseQuat::ZeroValue.Value));
const FVoxelDatabaseInterpCurveQuat FVoxelDatabaseInterpCurveQuat::ZeroValue = FInterpCurveQuat();
const FVoxelDatabaseInterpCurvePointTwoVectors FVoxelDatabaseInterpCurvePointTwoVectors::ZeroValue(FInterpCurvePointTwoVectors(0.0f, FVoxelDatabaseTwoVectors::ZeroValue.Value));
const FVoxelDatabaseInterpCurveTwoVectors FVoxelDatabaseInterpCurveTwoVectors::ZeroValue = FInterpCurveTwoVectors();
const FVoxelDatabaseInterpCurvePointLinearColor FVoxelDatabaseInterpCurvePointLinearColor::ZeroValue(FInterpCurvePointLinearColor(0.0f, FVoxelDatabaseLinearColor::ZeroValue.Value));
const FVoxelDatabaseInterpCurveLinearColor FVoxelDatabaseInterpCurveLinearColor::ZeroValue = FInterpCurveLinearColor();
const FVoxelDatabaseTransform FVoxelDatabaseTransform::ZeroValue(FTransform(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseRandomStream FVoxelDatabaseRandomStream::ZeroValue(FRandomStream(0));
const FVoxelDatabaseDateTime FVoxelDatabaseDateTime::ZeroValue(FDateTime(0));
const FVoxelDatabaseTimespan FVoxelDatabaseTimespan::ZeroValue(FTimespan(0));
const FVoxelDatabaseStringAssetReference FVoxelDatabaseStringAssetReference::ZeroValue = FStringAssetReference();
const FVoxelDatabaseStringClassReference FVoxelDatabaseStringClassReference::ZeroValue = FStringClassReference();
const FVoxelDatabaseString FVoxelDatabaseString::ZeroValue(FString(TEXT("")));
const FVoxelDatabaseName FVoxelDatabaseName::ZeroValue = FName();
const FVoxelDatabaseText FVoxelDatabaseText::ZeroValue(FText::FromString(FVoxelDatabaseString::ZeroValue.Value));
const FVoxelDatabaseAffineMap FVoxelDatabaseAffineMap::ZeroValue(FAffineMap(FVoxelDatabasePlane::ZeroValue.Value, FVoxelDatabasePlane::ZeroValue.Value, FVoxelDatabasePlane::ZeroValue.Value, FVoxelDatabasePlane::ZeroValue.Value));
const FVoxelDatabaseUnitaryMap FVoxelDatabaseUnitaryMap::ZeroValue(FUnitaryMap(FVoxelDatabaseVector::ZeroValue.Value, 0.0f));
const FVoxelDatabaseScaleMap FVoxelDatabaseScaleMap::ZeroValue(FScaleMap(FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseUniformScaleMap FVoxelDatabaseUniformScaleMap::ZeroValue(FUniformScaleMap(0.0f));
const FVoxelDatabaseTranslationMap FVoxelDatabaseTranslationMap::ZeroValue(FTranslationMap(FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseScaleTranslationMap FVoxelDatabaseScaleTranslationMap::ZeroValue(FScaleTranslationMap(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseUniformScaleTranslationMap FVoxelDatabaseUniformScaleTranslationMap::ZeroValue(FUniformScaleTranslationMap(0.0f, FVoxelDatabaseVector::ZeroValue.Value));
const FVoxelDatabaseNonlinearFrustumMap FVoxelDatabaseNonlinearFrustumMap::ZeroValue(FNonlinearFrustumMap(FVoxelDatabaseVector::ZeroValue.Value, FVoxelDatabaseVector::ZeroValue.Value, 0.0f, 0.0f));