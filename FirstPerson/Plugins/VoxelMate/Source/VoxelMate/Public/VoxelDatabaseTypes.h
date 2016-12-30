#pragma once
#include "EngineMinimal.h"
#include "GridCoordinateTransforms.h"
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

USTRUCT()
struct VOXELMATE_API FVoxelBase
{
	GENERATED_BODY()

	FVoxelBase()
		: VoxelType((EVoxelType)0), SurfaceIntersection(0)
	{}

	FVoxelBase(EVoxelType Type)
		: VoxelType(Type), SurfaceIntersection(0)
	{}

	UPROPERTY(BlueprintReadOnly)
		EVoxelType VoxelType;
	UPROPERTY()
		uint8 SurfaceIntersection;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelBool : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelBool()
		: FVoxelBase(EVoxelType::Bool), Value(ZeroValue.Value)
	{}

	explicit FVoxelBool(const bool& InValue)
		: FVoxelBase(EVoxelType::Bool), Value(InValue)
	{}

	FVoxelBool(const FVoxelBool& InValue)
		: FVoxelBase(EVoxelType::Bool), Value(InValue.Value)
	{}

	typedef bool ValueType;
	UPROPERTY(BlueprintReadWrite)
		bool Value;
	const static FVoxelBool ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelUInt8 : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelUInt8()
		: FVoxelBase(EVoxelType::UInt8), Value(ZeroValue.Value)
	{}

	explicit FVoxelUInt8(const uint8& InValue)
		: FVoxelBase(EVoxelType::UInt8), Value(InValue)
	{}

	FVoxelUInt8(const FVoxelUInt8& InValue)
		: FVoxelBase(EVoxelType::UInt8), Value(InValue.Value)
	{}

	typedef uint8 ValueType;
	UPROPERTY(BlueprintReadWrite)
		uint8 Value;
	const static FVoxelUInt8 ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelInt32 : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelInt32()
        : FVoxelBase(EVoxelType::Int32), Value(ZeroValue.Value)
    {}

	explicit FVoxelInt32(const int32& InValue)
		: FVoxelBase(EVoxelType::Int32), Value(InValue)
	{}

	FVoxelInt32(const FVoxelInt32& InValue)
		: FVoxelBase(EVoxelType::Int32), Value(InValue.Value)
	{}

	typedef int32 ValueType;
	UPROPERTY(BlueprintReadWrite)
		int32 Value;
    const static FVoxelInt32 ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelFloat : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelFloat()
		: FVoxelBase(EVoxelType::Float), Value(ZeroValue.Value)
	{}

	explicit FVoxelFloat(const float& InValue)
		: FVoxelBase(EVoxelType::Float), Value(InValue)
	{}

	FVoxelFloat(const FVoxelFloat& InValue)
		: FVoxelBase(EVoxelType::Float), Value(InValue.Value)
	{}

	typedef float ValueType;
	UPROPERTY(BlueprintReadWrite)
		float Value;
	const static FVoxelFloat ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelVector : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelVector()
		: FVoxelBase(EVoxelType::Vector), Value(ZeroValue.Value)
	{}

	explicit FVoxelVector(const FVector& InValue)
		: FVoxelBase(EVoxelType::Vector), Value(InValue)
	{}

	explicit FVoxelVector(EForceInit ForceInit)
		: FVoxelBase(EVoxelType::Vector), Value(ForceInit)
	{}

	FVoxelVector(const FVoxelVector& InValue)
		: FVoxelBase(EVoxelType::Vector), Value(InValue.Value)
	{}

	typedef FVector ValueType;
	UPROPERTY(BlueprintReadWrite)
		FVector Value;
	const static FVoxelVector ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelIntVector : public FVoxelBase
{
	GENERATED_BODY()

	explicit FVoxelIntVector()
		: FVoxelBase(EVoxelType::IntVector), Value(ZeroValue.Value)
	{}

	explicit FVoxelIntVector(const FIntVector& InValue)
		: FVoxelBase(EVoxelType::IntVector), Value(InValue)
	{}

	explicit FVoxelIntVector(EForceInit ForceInit)
		: FVoxelBase(EVoxelType::IntVector), Value(ForceInit)
	{}

	FVoxelIntVector(const FVoxelIntVector& InValue)
		: FVoxelBase(EVoxelType::IntVector), Value(InValue.Value)
	{}

	typedef FIntVector ValueType;
	UPROPERTY(BlueprintReadWrite)
		FIntVector Value;
	const static FVoxelIntVector ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataBool
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
	UPROPERTY(BlueprintReadWrite)
		bool Value;
	const static FMetadataBool ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataFloat
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
	UPROPERTY(BlueprintReadWrite)
		float Value;
	const static FMetadataFloat ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataInt32
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
	UPROPERTY(BlueprintReadWrite)
		int32 Value;
	const static FMetadataInt32 ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataUInt8
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
	UPROPERTY(BlueprintReadWrite)
		uint8 Value;
	const static FMetadataUInt8 ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataVector
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
	UPROPERTY(BlueprintReadWrite)
		FVector Value;
	const static FMetadataVector ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataColor
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
	UPROPERTY(BlueprintReadWrite)
		FColor Value;
	const static FMetadataColor ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataLinearColor
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
	UPROPERTY(BlueprintReadWrite)
		FLinearColor Value;
	const static FMetadataLinearColor ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataIntVector
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
	UPROPERTY(BlueprintReadWrite)
		FIntVector Value;
	const static FMetadataIntVector ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataRotator
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
	UPROPERTY(BlueprintReadWrite)
		FRotator Value;
	const static FMetadataRotator ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataQuat
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
	UPROPERTY(BlueprintReadWrite)
		FQuat Value;
	const static FMetadataQuat ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataBox
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
	UPROPERTY(BlueprintReadWrite)
		FBox Value;
	const static FMetadataBox ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataDateTime
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
	UPROPERTY(BlueprintReadWrite)
		FDateTime Value;
	const static FMetadataDateTime ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataString
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
	UPROPERTY(BlueprintReadWrite)
		FString Value;
	const static FMetadataString ZeroValue;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FMetadataText
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
	UPROPERTY(BlueprintReadWrite)
		FText Value;
	const static FMetadataText ZeroValue;
};
