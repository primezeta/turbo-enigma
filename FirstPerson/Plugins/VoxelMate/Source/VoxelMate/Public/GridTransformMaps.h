#pragma once
#include "EngineMinimal.h"
#include "Math/Plane.h"
#include "Math/Quat.h"
#include "Math/Box.h"
#include "Math/Matrix.h"
#include "Math/ScaleMatrix.h"
#include "Math/TranslationMatrix.h"
#include "Math/RotationTranslationMatrix.h"

struct FAffineMap;
struct FUnitaryMap;
struct FScaleMap;
struct FUniformScaleMap;
struct FTranslationMap;
struct FScaleTranslationMap;
struct FUniformScaleTranslationMap;
struct FNonlinearFrustumMap;

struct VOXELMATE_API FAffineMap : public FMatrix
{
    FAffineMap()
        : FMatrix(FPlane(EForceInit::ForceInitToZero), FPlane(EForceInit::ForceInitToZero), FPlane(EForceInit::ForceInitToZero), FPlane(EForceInit::ForceInitToZero))
    {}

    FAffineMap(const FPlane& InX, const FPlane& InY, const FPlane& InZ, const FPlane& InW)
        : FMatrix(InX, InY, InZ, InW)
    {}
};

struct VOXELMATE_API FUnitaryMap : public FQuat
{
    FUnitaryMap()
        : FQuat(FVector::ZeroVector, 0.0f)
    {}

    FUnitaryMap(const FVector& Axis, float AngleRad)
        : FQuat(Axis, AngleRad)
    {}
};

struct VOXELMATE_API FScaleMap : public FScaleMatrix
{
    FScaleMap()
        : FScaleMatrix(FVector::ZeroVector)
    {}

    FScaleMap(const FVector& Scale)
        : FScaleMatrix(Scale)
    {}
};

struct VOXELMATE_API FUniformScaleMap : public FScaleMatrix
{
    FUniformScaleMap()
        : FScaleMatrix(0.0f)
    {}

    FUniformScaleMap(float Scale)
        : FScaleMatrix(Scale)
    {}
};

struct VOXELMATE_API FTranslationMap : public FTranslationMatrix
{
    FTranslationMap()
        : FTranslationMatrix(FVector::ZeroVector)
    {}

    FTranslationMap(const FVector& Delta)
        : FTranslationMatrix(Delta)
    {}
};

struct VOXELMATE_API FScaleTranslationMap : public FScaleMatrix, public FTranslationMatrix
{
    FScaleTranslationMap()
        : FScaleMatrix(FVector::ZeroVector), FTranslationMatrix(FVector::ZeroVector)
    {}

    FScaleTranslationMap(const FVector& Scale, const FVector& Delta)
        : FScaleMatrix(Scale), FTranslationMatrix(Delta)
    {}
};

struct VOXELMATE_API FUniformScaleTranslationMap : public FScaleMatrix, public FTranslationMatrix
{
    FUniformScaleTranslationMap()
        : FScaleMatrix(0.0f), FTranslationMatrix(FVector::ZeroVector)
    {}

    FUniformScaleTranslationMap(float Scale, const FVector& Delta)
        : FScaleMatrix(Scale), FTranslationMatrix(Delta)
    {}
};

struct VOXELMATE_API FNonlinearFrustumMap : public FBox
{
    FNonlinearFrustumMap()
        : FBox(FVector::ZeroVector, FVector::ZeroVector), Taper(0.0f), Depth(0.0f)
    {}

    FNonlinearFrustumMap(const FVector& BBoxMin, const FVector& BBoxMax, float FrustumTaper, float FrustumDepth)
        : FBox(BBoxMin, BBoxMax), Taper(FrustumTaper), Depth(FrustumDepth)
    {}

    float Taper;
    float Depth;
};
