#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"

const FVoxelBool FVoxelBool::ZeroValue(false);
const FVoxelUInt8 FVoxelUInt8::ZeroValue(0);
const FVoxelInt32 FVoxelInt32::ZeroValue(0);
const FVoxelFloat FVoxelFloat::ZeroValue(0.0f);
const FVoxelVector FVoxelVector::ZeroValue(EForceInit::ForceInitToZero);
const FVoxelIntVector FVoxelIntVector::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataBool FMetadataBool::ZeroValue(false);
const FMetadataFloat FMetadataFloat::ZeroValue(0.0f);
const FMetadataInt32 FMetadataInt32::ZeroValue(0);
const FMetadataUInt8 FMetadataUInt8::ZeroValue(0);
const FMetadataVector FMetadataVector::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataColor FMetadataColor::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataLinearColor FMetadataLinearColor::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataIntVector FMetadataIntVector::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataRotator FMetadataRotator::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataQuat FMetadataQuat::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataBox FMetadataBox::ZeroValue(EForceInit::ForceInitToZero);
const FMetadataDateTime FMetadataDateTime::ZeroValue(0);
const FMetadataString FMetadataString::ZeroValue(TEXT(""));
const FMetadataText FMetadataText::ZeroValue(TEXT(""));
const FMetadataAffineCoordinateTransform FMetadataAffineCoordinateTransform::ZeroValue(FAffineCoordinateTransform(FVector(1.0f,0.0f,0.0f), FVector(0.0f,1.0f,0.0f), FVector(0.0f,0.0f,1.0f)));
const FMetadataUnitaryCoordinateTransform FMetadataUnitaryCoordinateTransform::ZeroValue(FUnitaryCoordinateTransform(FVector(EForceInit::ForceInitToZero), 0.0f));
const FMetadataScaleCoordinateTransform FMetadataScaleCoordinateTransform::ZeroValue(FScaleCoordinateTransform(FVector(1.0f)));
const FMetadataUniformScaleCoordinateTransform FMetadataUniformScaleCoordinateTransform::ZeroValue(FUniformScaleCoordinateTransform(1.0f));
const FMetadataTranslationCoordinateTransform FMetadataTranslationCoordinateTransform::ZeroValue(FVector(0.0f));
const FMetadataScaleTranslationCoordinateTransform FMetadataScaleTranslationCoordinateTransform::ZeroValue(FScaleTranslationCoordinateTransform(FVector(1.0f), FVector(EForceInit::ForceInitToZero)));
const FMetadataUniformScaleTranslationCoordinateTransform FMetadataUniformScaleTranslationCoordinateTransform::ZeroValue(FUniformScaleTranslationCoordinateTransform(1.0f, FVector(EForceInit::ForceInitToZero)));
const FMetadataNonlinearFrustumCoordinateTransform FMetadataNonlinearFrustumCoordinateTransform::ZeroValue(FNonlinearFrustumCoordinateTransform(FVector(EForceInit::ForceInitToZero), FVector(EForceInit::ForceInitToZero), 0.0f, 0.0f));
