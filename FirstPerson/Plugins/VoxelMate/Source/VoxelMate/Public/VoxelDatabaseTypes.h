#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseTypes.generated.h"

struct FVoxelBool;
struct FVoxelUInt8;
struct FVoxelInt32;
struct FVoxelFloat;
struct FVoxelVector;
struct FVoxelIntVector;
struct FMetadataBool;
struct FMetadataFloat;
struct FMetadataInt32;
struct FMetadataUInt8;
struct FMetadataVector;
struct FMetadataColor;
struct FMetadataLinearColor;
struct FMetadataIntVector;
struct FMetadataRotator;
struct FMetadataQuat;
struct FMetadataBox;
struct FMetadataDateTime;
struct FMetadataString;
struct FMetadataText;
struct FMetadataAffineCoordinateTransform;
struct FMetadataUnitaryCoordinateTransform;
struct FMetadataScaleCoordinateTransform;
struct FMetadataUniformScaleCoordinateTransform;
struct FMetadataTranslationCoordinateTransform;
struct FMetadataScaleTranslationCoordinateTransform;
struct FMetadataUniformScaleTranslationCoordinateTransform;
struct FMetadataNonlinearFrustumCoordinateTransform;

USTRUCT(BlueprintType)
struct FVoxelBool
{
	GENERATED_BODY()

	explicit FVoxelBool()
		: Value(ZeroValue.Value)
	{}

	explicit FVoxelBool(const bool& InValue)
		: Value(InValue)
	{}

	FVoxelBool(const FVoxelBool& InValue)
		: Value(InValue.Value)
	{}

	typedef bool ValueType;
	bool Value;
	const static FVoxelBool ZeroValue;
};

USTRUCT(BlueprintType)
struct FVoxelUInt8
{
	GENERATED_BODY()

	explicit FVoxelUInt8()
		: Value(ZeroValue.Value)
	{}

	explicit FVoxelUInt8(const uint8& InValue)
		: Value(InValue)
	{}

	FVoxelUInt8(const FVoxelUInt8& InValue)
		: Value(InValue.Value)
	{}

	typedef uint8 ValueType;
	uint8 Value;
	const static FVoxelUInt8 ZeroValue;
};

USTRUCT(BlueprintType)
struct FVoxelInt32
{
	GENERATED_BODY()

	explicit FVoxelInt32()
        : Value(ZeroValue.Value)
    {}

	explicit FVoxelInt32(const int32& InValue)
		: Value(InValue)
	{}

	FVoxelInt32(const FVoxelInt32& InValue)
		: Value(InValue.Value)
	{}

	typedef int32 ValueType;
	int32 Value;
    const static FVoxelInt32 ZeroValue;
};

USTRUCT(BlueprintType)
struct FVoxelFloat
{
	GENERATED_BODY()

	explicit FVoxelFloat()
		: Value(ZeroValue.Value)
	{}

	explicit FVoxelFloat(const float& InValue)
		: Value(InValue)
	{}

	FVoxelFloat(const FVoxelFloat& InValue)
		: Value(InValue.Value)
	{}

	typedef float ValueType;
	float Value;
	const static FVoxelFloat ZeroValue;
};

USTRUCT(BlueprintType)
struct FVoxelVector
{
	GENERATED_BODY()

	explicit FVoxelVector()
		: Value(ZeroValue.Value)
	{}

	explicit FVoxelVector(const FVector& InValue)
		: Value(InValue)
	{}

	explicit FVoxelVector(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FVoxelVector(const FVoxelVector& InValue)
		: Value(InValue.Value)
	{}

	typedef FVector ValueType;
	FVector Value;
	const static FVoxelVector ZeroValue;
};

USTRUCT(BlueprintType)
struct FVoxelIntVector
{
	GENERATED_BODY()

	explicit FVoxelIntVector()
		: Value(ZeroValue.Value)
	{}

	explicit FVoxelIntVector(const FIntVector& InValue)
		: Value(InValue)
	{}

	explicit FVoxelIntVector(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FVoxelIntVector(const FVoxelIntVector& InValue)
		: Value(InValue.Value)
	{}

	typedef FIntVector ValueType;
	FIntVector Value;
	const static FVoxelIntVector ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataBool
{
	GENERATED_BODY()

	explicit FMetadataBool()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataBool(const bool& InValue)
		: Value(InValue)
	{}

	FMetadataBool(const FMetadataBool& InValue)
		: Value(InValue.Value)
	{}

	typedef bool ValueType;
	bool Value;
	const static FMetadataBool ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataFloat
{
	GENERATED_BODY()

	explicit FMetadataFloat()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataFloat(const float& InValue)
		: Value(InValue)
	{}

	FMetadataFloat(const FMetadataFloat& InValue)
		: Value(InValue.Value)
	{}

	typedef float ValueType;
	float Value;
	const static FMetadataFloat ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataInt32
{
	GENERATED_BODY()

	explicit FMetadataInt32()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataInt32(const int32& InValue)
		: Value(InValue)
	{}

	FMetadataInt32(const FMetadataInt32& InValue)
		: Value(InValue.Value)
	{}

	typedef int32 ValueType;
	int32 Value;
	const static FMetadataInt32 ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataUInt8
{
	GENERATED_BODY()

	explicit FMetadataUInt8()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataUInt8(const uint8& InValue)
		: Value(InValue)
	{}

	FMetadataUInt8(const FMetadataUInt8& InValue)
		: Value(InValue.Value)
	{}

	typedef uint8 ValueType;
	uint8 Value;
	const static FMetadataUInt8 ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataVector
{
	GENERATED_BODY()

	explicit FMetadataVector()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataVector(const FVector& InValue)
		: Value(InValue)
	{}

	explicit FMetadataVector(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataVector(const FMetadataVector& InValue)
		: Value(InValue.Value)
	{}

	typedef FVector ValueType;
	FVector Value;
	const static FMetadataVector ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataColor
{
	GENERATED_BODY()

	explicit FMetadataColor()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataColor(const FColor& InValue)
		: Value(InValue)
	{}

	explicit FMetadataColor(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataColor(const FMetadataColor& InValue)
		: Value(InValue.Value)
	{}

	typedef FColor ValueType;
	FColor Value;
	const static FMetadataColor ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataLinearColor
{
	GENERATED_BODY()

	explicit FMetadataLinearColor()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataLinearColor(const FLinearColor& InValue)
		: Value(InValue)
	{}

	explicit FMetadataLinearColor(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataLinearColor(const FMetadataLinearColor& InValue)
		: Value(InValue.Value)
	{}

	typedef FLinearColor ValueType;
	FLinearColor Value;
	const static FMetadataLinearColor ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataIntVector
{
	GENERATED_BODY()

	explicit FMetadataIntVector()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataIntVector(const FIntVector& InValue)
		: Value(InValue)
	{}

	explicit FMetadataIntVector(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataIntVector(const FMetadataIntVector& InValue)
		: Value(InValue.Value)
	{}

	typedef FIntVector ValueType;
	FIntVector Value;
	const static FMetadataIntVector ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataRotator
{
	GENERATED_BODY()

	explicit FMetadataRotator()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataRotator(const FRotator& InValue)
		: Value(InValue)
	{}

	explicit FMetadataRotator(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataRotator(const FMetadataRotator& InValue)
		: Value(InValue.Value)
	{}

	typedef FRotator ValueType;
	FRotator Value;
	const static FMetadataRotator ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataQuat
{
	GENERATED_BODY()

	explicit FMetadataQuat()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataQuat(const FQuat& InValue)
		: Value(InValue)
	{}

	explicit FMetadataQuat(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataQuat(const FMetadataQuat& InValue)
		: Value(InValue.Value)
	{}

	typedef FQuat ValueType;
	FQuat Value;
	const static FMetadataQuat ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataBox
{
	GENERATED_BODY()

	explicit FMetadataBox()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataBox(const FBox& InValue)
		: Value(InValue)
	{}

	explicit FMetadataBox(EForceInit ForceInit)
		: Value(ForceInit)
	{}

	FMetadataBox(const FMetadataBox& InValue)
		: Value(InValue.Value)
	{}

	typedef FBox ValueType;
	FBox Value;
	const static FMetadataBox ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataDateTime
{
	GENERATED_BODY()

	explicit FMetadataDateTime()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataDateTime(const FDateTime& InValue)
		: Value(InValue)
	{}

	FMetadataDateTime(const FMetadataDateTime& InValue)
		: Value(InValue.Value)
	{}

	typedef FDateTime ValueType;
	FDateTime Value;
	const static FMetadataDateTime ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataString
{
	GENERATED_BODY()

	explicit FMetadataString()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataString(const FString& InValue)
		: Value(InValue)
	{}

	FMetadataString(const FMetadataString& InValue)
		: Value(InValue.Value)
	{}

	typedef FString ValueType;
	FString Value;
	const static FMetadataString ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataText
{
	GENERATED_BODY()

	explicit FMetadataText()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataText(const FText& InValue)
		: Value(InValue)
	{}

	FMetadataText(const FString& InValue)
		: Value(FText::FromString(InValue))
	{}

	FMetadataText(const FMetadataText& InValue)
		: Value(InValue.Value)
	{}

	typedef FText ValueType;
	FText Value;
	const static FMetadataText ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataAffineCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataAffineCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataAffineCoordinateTransform(const FAffineCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataAffineCoordinateTransform(const FMetadataAffineCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FAffineCoordinateTransform ValueType;
	FAffineCoordinateTransform Value;
	const static FMetadataAffineCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataUnitaryCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataUnitaryCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataUnitaryCoordinateTransform(const FUnitaryCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataUnitaryCoordinateTransform(const FMetadataUnitaryCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FUnitaryCoordinateTransform ValueType;
	FUnitaryCoordinateTransform Value;
	const static FMetadataUnitaryCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataScaleCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataScaleCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataScaleCoordinateTransform(const FScaleCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataScaleCoordinateTransform(const FMetadataScaleCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FScaleCoordinateTransform ValueType;
	FScaleCoordinateTransform Value;
	const static FMetadataScaleCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataUniformScaleCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataUniformScaleCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataUniformScaleCoordinateTransform(const FUniformScaleCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataUniformScaleCoordinateTransform(const FMetadataUniformScaleCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FUniformScaleCoordinateTransform ValueType;
	FUniformScaleCoordinateTransform Value;
	const static FMetadataUniformScaleCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataTranslationCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataTranslationCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataTranslationCoordinateTransform(const FTranslationCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataTranslationCoordinateTransform(const FMetadataTranslationCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FTranslationCoordinateTransform ValueType;
	FTranslationCoordinateTransform Value;
	const static FMetadataTranslationCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataScaleTranslationCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataScaleTranslationCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataScaleTranslationCoordinateTransform(const FScaleTranslationCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataScaleTranslationCoordinateTransform(const FMetadataScaleTranslationCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FScaleTranslationCoordinateTransform ValueType;
	FScaleTranslationCoordinateTransform Value;
	const static FMetadataScaleTranslationCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataUniformScaleTranslationCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataUniformScaleTranslationCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataUniformScaleTranslationCoordinateTransform(const FUniformScaleTranslationCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataUniformScaleTranslationCoordinateTransform(const FMetadataUniformScaleTranslationCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FUniformScaleTranslationCoordinateTransform ValueType;
	FUniformScaleTranslationCoordinateTransform Value;
	const static FMetadataUniformScaleTranslationCoordinateTransform ZeroValue;
};

USTRUCT(BlueprintType)
struct FMetadataNonlinearFrustumCoordinateTransform
{
	GENERATED_BODY()

	explicit FMetadataNonlinearFrustumCoordinateTransform()
		: Value(ZeroValue.Value)
	{}

	explicit FMetadataNonlinearFrustumCoordinateTransform(const FNonlinearFrustumCoordinateTransform& InValue)
		: Value(InValue)
	{}

	FMetadataNonlinearFrustumCoordinateTransform(const FMetadataNonlinearFrustumCoordinateTransform& InValue)
		: Value(InValue.Value)
	{}

	typedef FNonlinearFrustumCoordinateTransform ValueType;
	FNonlinearFrustumCoordinateTransform Value;
	const static FMetadataNonlinearFrustumCoordinateTransform ZeroValue;
};
