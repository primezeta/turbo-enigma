#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "Math/Plane.h"
#include "Math/Quat.h"
#include "Math/Box.h"
#include "Math/Matrix.h"
#include "Math/TranslationMatrix.h"
#include "GridCoordinateTransforms.generated.h"

USTRUCT(BlueprintType)
struct VOXELMATE_API FAffineCoordinateTransform
{
	GENERATED_BODY()

	FAffineCoordinateTransform()
	{}

	//According to Matrix.h: "Matrix elements are accessed with M[RowIndex][ColumnIndex]"
	//In OpenVDB, matrices are in row-order form and a valid affine map defined from a 4d-matrix requires column 3 = 0, 0, 0, 1
	//Therefore the only valid FMatrix constructor is the following:
	FAffineCoordinateTransform(const FVector& InX, const FVector& InY, const FVector& InZ)
		: Matrix(InX, InY, InZ, FVector(0.0f, 0.0f, 1.0f))
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		Matrix.Serialize(Ar);
	}

	FMatrix Matrix;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FUnitaryCoordinateTransform
{
	GENERATED_BODY()

	FUnitaryCoordinateTransform()
		: Quat(FQuat::Identity)
	{}

	FUnitaryCoordinateTransform(const FVector& Axis, float AngleRadians)
		: Quat(Axis, AngleRadians)
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		Quat.Serialize(Ar);
	}

	FQuat Quat;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FScaleCoordinateTransform
{
	GENERATED_BODY()

	FScaleCoordinateTransform()
		: ScaleVec(FVector(1.0f))
	{}

	FScaleCoordinateTransform(const FVector& Scale)
		: ScaleVec(Scale.GetAbs())
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		ScaleVec.Serialize(Ar);
	}

	FVector ScaleVec;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FUniformScaleCoordinateTransform
{
	GENERATED_BODY()

	FUniformScaleCoordinateTransform()
		: ScaleValue(1.0f)
	{}

	FUniformScaleCoordinateTransform(float Scale)
		: ScaleValue(FMath::Abs(Scale))
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		Ar << ScaleValue;
	}

	float ScaleValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FTranslationCoordinateTransform
{
	GENERATED_BODY()

	FTranslationCoordinateTransform()
		: TranslationVec(FVector::ZeroVector)
	{}

	FTranslationCoordinateTransform(const FVector& Translation)
		: TranslationVec(Translation)
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		TranslationVec.Serialize(Ar);
	}

	FVector TranslationVec;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FScaleTranslationCoordinateTransform
{
	GENERATED_BODY()

	FScaleTranslationCoordinateTransform()
		: ScaleVec(FVector(1.0f)), TranslationVec(FVector::ZeroVector)
	{}

	FScaleTranslationCoordinateTransform(const FVector& Scale, const FVector& Translation)
		: ScaleVec(Scale.GetAbs()), TranslationVec(Translation)
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		ScaleVec.Serialize(Ar);
		TranslationVec.Serialize(Ar);
	}

	FVector ScaleVec;
	FVector TranslationVec;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FUniformScaleTranslationCoordinateTransform
{
	GENERATED_BODY()

	FUniformScaleTranslationCoordinateTransform()
		: ScaleValue(1.0f), TranslationVec(FVector::ZeroVector)
	{}

	FUniformScaleTranslationCoordinateTransform(float Scale, const FVector& Translation)
		: ScaleValue(FMath::Abs(Scale)), TranslationVec(Translation)
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		Ar << ScaleValue;
		TranslationVec.Serialize(Ar);
	}

	float ScaleValue;
	FVector TranslationVec;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FNonlinearFrustumCoordinateTransform
{
	GENERATED_BODY()

	FNonlinearFrustumCoordinateTransform()
		: Box(FVector::ZeroVector, FVector::ZeroVector), Taper(0.0f), Depth(0.0f)
	{}

	FNonlinearFrustumCoordinateTransform(const FVector& BoxMin, const FVector& BoxMax, float FrustumTaper, float FrustumDepth)
		: Box(BoxMin, BoxMax), Taper(FMath::Abs(FrustumTaper)), Depth(FMath::Abs(FrustumDepth))
	{}

	VOXELMATEINLINE void Serialize(FArchive& Ar)
	{
		Box.Serialize(Ar);
		Ar << Taper;
		Ar << Depth;
	}

	FBox Box;
	float Taper;
	float Depth;
};
