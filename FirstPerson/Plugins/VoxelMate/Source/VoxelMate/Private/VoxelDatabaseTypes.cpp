#include "VoxelMatePCH.h"
#include "VoxelDatabaseTypes.h"

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelBool& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelBool& InLhs, const FVoxelBool& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE bool operator<(const FVoxelBool& InLhs, const FVoxelBool& InRhs)
{
	return !InLhs.Value && InRhs.Value;
}

VOXELMATEINLINE bool operator>(const FVoxelBool& InLhs, const FVoxelBool& InRhs)
{
	return InLhs.Value && !InRhs.Value;
}

VOXELMATEINLINE FVoxelBool operator+(const FVoxelBool& InLhs, const FVoxelBool& InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelBool operator-(const FVoxelBool& InLhs, const FVoxelBool& InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelBool operator+(const FVoxelBool& InLhs, const float &InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelBool Abs(const FVoxelBool& InValue)
{
	return InValue;
}

VOXELMATEINLINE FVoxelBool operator-(const FVoxelBool& InValue)
{
	return InValue;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelUInt8& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelUInt8& InLhs, const FVoxelUInt8& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE bool operator<(const FVoxelUInt8& InLhs, const FVoxelUInt8& InRhs)
{
	return InLhs.Value < InRhs.Value;
}

VOXELMATEINLINE bool operator>(const FVoxelUInt8& InLhs, const FVoxelUInt8& InRhs)
{
	return InLhs.Value > InRhs.Value;
}

VOXELMATEINLINE FVoxelUInt8 operator+(const FVoxelUInt8& InLhs, const FVoxelUInt8& InRhs)
{
	return FVoxelUInt8(InLhs.Value + InRhs.Value);
}

VOXELMATEINLINE FVoxelUInt8 operator-(const FVoxelUInt8& InLhs, const FVoxelUInt8& InRhs)
{
	return FVoxelUInt8(InLhs.Value - InRhs.Value);
}

VOXELMATEINLINE FVoxelUInt8 operator+(const FVoxelUInt8& InLhs, const float &InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelUInt8 Abs(const FVoxelUInt8& InValue)
{
	return InValue;
}

VOXELMATEINLINE FVoxelUInt8 operator-(const FVoxelUInt8& InValue)
{
	return InValue;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelInt32& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelInt32& InLhs, const FVoxelInt32& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE bool operator<(const FVoxelInt32& InLhs, const FVoxelInt32& InRhs)
{
	return InLhs.Value < InRhs.Value;
}

VOXELMATEINLINE bool operator>(const FVoxelInt32& InLhs, const FVoxelInt32& InRhs)
{
	return InLhs.Value > InRhs.Value;
}

VOXELMATEINLINE FVoxelInt32 operator+(const FVoxelInt32& InLhs, const FVoxelInt32& InRhs)
{
	return FVoxelInt32(InLhs.Value + InRhs.Value);
}

VOXELMATEINLINE FVoxelInt32 operator-(const FVoxelInt32& InLhs, const FVoxelInt32& InRhs)
{
	return FVoxelInt32(InLhs.Value - InRhs.Value);
}

VOXELMATEINLINE FVoxelInt32 operator+(const FVoxelInt32& InLhs, const float &InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelInt32 Abs(const FVoxelInt32& InValue)
{
	return FVoxelInt32(FMath::Abs(InValue.Value));
}

VOXELMATEINLINE FVoxelInt32 operator-(const FVoxelInt32& InValue)
{
	return FVoxelInt32(-InValue.Value);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelFloat& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelFloat& InLhs, const FVoxelFloat& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value, InRhs.Value);
}

VOXELMATEINLINE bool operator<(const FVoxelFloat& InLhs, const FVoxelFloat& InRhs)
{
	return InLhs.Value < InRhs.Value;
}

VOXELMATEINLINE bool operator>(const FVoxelFloat& InLhs, const FVoxelFloat& InRhs)
{
	return InLhs.Value > InRhs.Value;
}

VOXELMATEINLINE FVoxelFloat operator+(const FVoxelFloat& InLhs, const FVoxelFloat& InRhs)
{
	return FVoxelFloat(InLhs.Value + InRhs.Value);
}

VOXELMATEINLINE FVoxelFloat operator-(const FVoxelFloat& InLhs, const FVoxelFloat& InRhs)
{
	return FVoxelFloat(InLhs.Value - InRhs.Value);
}

VOXELMATEINLINE FVoxelFloat operator+(const FVoxelFloat& InLhs, const float &InRhs)
{
	return FVoxelFloat(InLhs.Value + InRhs);
}

VOXELMATEINLINE FVoxelFloat Abs(const FVoxelFloat& InValue)
{
	return FVoxelFloat(FMath::Abs(InValue.Value));
}

VOXELMATEINLINE FVoxelFloat operator-(const FVoxelFloat& InValue)
{
	return FVoxelFloat(-InValue.Value);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelVector& InLhs, const FVoxelVector& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value.X, InRhs.Value.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Y, InRhs.Value.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Z, InRhs.Value.Z);
}

VOXELMATEINLINE bool operator<(const FVoxelVector& InLhs, const FVoxelVector& InRhs)
{
	return (!FMath::IsNearlyEqual(InLhs.Value.X, InRhs.Value.X) && InLhs.Value.X < InRhs.Value.X) ||
		   (!FMath::IsNearlyEqual(InLhs.Value.Y, InRhs.Value.Y) && InLhs.Value.Y < InRhs.Value.Y) ||
		   (!FMath::IsNearlyEqual(InLhs.Value.Z, InRhs.Value.Z) && InLhs.Value.Z < InRhs.Value.Z);
}

VOXELMATEINLINE bool operator>(const FVoxelVector& InLhs, const FVoxelVector& InRhs)
{
	return (!FMath::IsNearlyEqual(InLhs.Value.X, InRhs.Value.X) && InLhs.Value.X > InRhs.Value.X) ||
		   (!FMath::IsNearlyEqual(InLhs.Value.Y, InRhs.Value.Y) && InLhs.Value.Y > InRhs.Value.Y) ||
		   (!FMath::IsNearlyEqual(InLhs.Value.Z, InRhs.Value.Z) && InLhs.Value.Z > InRhs.Value.Z);
}

VOXELMATEINLINE FVoxelVector operator+(const FVoxelVector& InLhs, const FVoxelVector& InRhs)
{
	return FVoxelVector(InLhs.Value + InRhs.Value);
}

VOXELMATEINLINE FVoxelVector operator-(const FVoxelVector& InLhs, const FVoxelVector& InRhs)
{
	return FVoxelVector(InLhs.Value - InRhs.Value);
}

VOXELMATEINLINE FVoxelVector operator+(const FVoxelVector& InLhs, const float &InRhs)
{
	return FVoxelVector(InLhs.Value + InRhs);
}

VOXELMATEINLINE FVoxelVector Abs(const FVoxelVector& InValue)
{
	return FVoxelVector(InValue.Value.GetAbs());
}

VOXELMATEINLINE FVoxelVector operator-(const FVoxelVector& InValue)
{
	return FVoxelVector(FVector(-InValue.Value.X, -InValue.Value.Y, -InValue.Value.Z));
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FVoxelIntVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

VOXELMATEINLINE bool operator==(const FVoxelIntVector& InLhs, const FVoxelIntVector& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE bool operator<(const FVoxelIntVector& InLhs, const FVoxelIntVector& InRhs)
{
	return InLhs.Value.X < InRhs.Value.X ||
		   InLhs.Value.Y < InRhs.Value.Y ||
		   InLhs.Value.Z < InRhs.Value.Z;
}

VOXELMATEINLINE bool operator>(const FVoxelIntVector& InLhs, const FVoxelIntVector& InRhs)
{
	return InLhs.Value.X > InRhs.Value.X ||
		   InLhs.Value.Y > InRhs.Value.Y ||
		   InLhs.Value.Z > InRhs.Value.Z;
}

VOXELMATEINLINE FVoxelIntVector operator+(const FVoxelIntVector& InLhs, const FVoxelIntVector& InRhs)
{
	return FVoxelIntVector(InLhs.Value + InRhs.Value);
}

VOXELMATEINLINE FVoxelIntVector operator-(const FVoxelIntVector& InLhs, const FVoxelIntVector& InRhs)
{
	return FVoxelIntVector(InLhs.Value - InRhs.Value);
}

VOXELMATEINLINE FVoxelIntVector operator+(const FVoxelIntVector& InLhs, const float &InRhs)
{
	(void)InRhs;
	return InLhs;
}

VOXELMATEINLINE FVoxelIntVector Abs(const FVoxelIntVector& InValue)
{
	return FVoxelIntVector(FIntVector(FMath::Abs(InValue.Value.X), FMath::Abs(InValue.Value.Y), FMath::Abs(InValue.Value.Z)));
}

VOXELMATEINLINE FVoxelIntVector operator-(const FVoxelIntVector& InValue)
{
	return FVoxelIntVector(FIntVector(-InValue.Value.X, -InValue.Value.Y, -InValue.Value.Z));
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataBool& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataBool& InLhs, const FMetadataBool& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataFloat& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataFloat& InLhs, const FMetadataFloat& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value, InRhs.Value);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataInt32& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataInt32& InLhs, const FMetadataInt32& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataUInt8& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataUInt8& InLhs, const FMetadataUInt8& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataVector& InLhs, const FMetadataVector& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value.X, InRhs.Value.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Y, InRhs.Value.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Z, InRhs.Value.Z);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataColor& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.DWColor()), sizeof(uint32));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataColor& InLhs, const FMetadataColor& InRhs)
{
	return InLhs.Value.DWColor() == InRhs.Value.DWColor();
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataLinearColor& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.R), sizeof(InValue.Value.R));
	os.write(reinterpret_cast<const char*>(&InValue.Value.G), sizeof(InValue.Value.G));
	os.write(reinterpret_cast<const char*>(&InValue.Value.B), sizeof(InValue.Value.B));
	os.write(reinterpret_cast<const char*>(&InValue.Value.A), sizeof(InValue.Value.A));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataLinearColor& InLhs, const FMetadataLinearColor& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataIntVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataIntVector& InLhs, const FMetadataIntVector& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataRotator& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.Pitch), sizeof(InValue.Value.Pitch));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Roll), sizeof(InValue.Value.Roll));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Yaw), sizeof(InValue.Value.Yaw));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataRotator& InLhs, const FMetadataRotator& InRhs)
{
	return InLhs.Value.Equals(InRhs.Value);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataQuat& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	os.write(reinterpret_cast<const char*>(&InValue.Value.W), sizeof(InValue.Value.W));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataQuat& InLhs, const FMetadataQuat& InRhs)
{
	return InLhs.Value.Equals(InRhs.Value);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataBox& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.Min.X), sizeof(InValue.Value.Min.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Min.Y), sizeof(InValue.Value.Min.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Min.Z), sizeof(InValue.Value.Min.Z));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Max.X), sizeof(InValue.Value.Max.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Max.Y), sizeof(InValue.Value.Max.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Max.Z), sizeof(InValue.Value.Max.Z));
	os.write(reinterpret_cast<const char*>(&InValue.Value.IsValid), sizeof(InValue.Value.IsValid));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataBox& InLhs, const FMetadataBox& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value.Min.X, InRhs.Value.Min.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Min.Y, InRhs.Value.Min.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Min.Z, InRhs.Value.Min.Z) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.X, InRhs.Value.Max.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.Y, InRhs.Value.Max.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.Z, InRhs.Value.Max.Z);
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataDateTime& InValue)
{
	const int64 Ticks = InValue.Value.GetTicks();
	os.write(reinterpret_cast<const char*>(&Ticks), sizeof(uint64));
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataDateTime& InLhs, const FMetadataDateTime& InRhs)
{
	return InLhs.Value.GetTicks() == InRhs.Value.GetTicks();
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataString& InValue)
{
	os.write(reinterpret_cast<const char*>(InValue.Value.GetCharArray().GetData()), InValue.Value.GetAllocatedSize());
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataString& InLhs, const FMetadataString& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

VOXELMATEINLINE std::ostream& operator<<(std::ostream& os, const FMetadataText& InValue)
{
	const FString& SourceString = InValue.Value.ToString();
	os.write(reinterpret_cast<const char*>(SourceString.GetCharArray().GetData()), SourceString.GetAllocatedSize());
	return os;
}

VOXELMATEINLINE bool operator==(const FMetadataText& InLhs, const FMetadataText& InRhs)
{
	return InLhs.Value.EqualTo(InRhs.Value, ETextComparisonLevel::Default);
}
