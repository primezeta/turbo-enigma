#include "VoxelMatePCH.h"
#include "VoxelDatabaseTypes.h"

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FVoxel& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FVoxel& InLhs, const FVoxel& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value, InRhs.Value);
}

FORCEINLINE_DEBUGGABLE bool operator<(const FVoxel& InLhs, const FVoxel& InRhs)
{
	return InLhs.Value < InRhs.Value;
}

FORCEINLINE_DEBUGGABLE bool operator>(const FVoxel& InLhs, const FVoxel& InRhs)
{
	return InLhs.Value > InRhs.Value;
}

FORCEINLINE_DEBUGGABLE FVoxel operator+(const FVoxel& InLhs, const FVoxel& InRhs)
{
	return FVoxel(InLhs.Value + InRhs.Value);
}

FORCEINLINE_DEBUGGABLE FVoxel operator-(const FVoxel& InLhs, const FVoxel& InRhs)
{
	return FVoxel(InLhs.Value - InRhs.Value);
}

FORCEINLINE_DEBUGGABLE FVoxel operator+(const FVoxel& InLhs, const float &InRhs)
{
	return FVoxel(InLhs.Value + InRhs);
}

FORCEINLINE_DEBUGGABLE FVoxel Abs(const FVoxel& InValue)
{
	return FVoxel(FMath::Abs(InValue.Value));
}

FORCEINLINE_DEBUGGABLE FVoxel operator-(const FVoxel& InValue)
{
	return FVoxel(-InValue.Value);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataBool& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataBool& InLhs, const FMetadataBool& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataFloat& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataFloat& InLhs, const FMetadataFloat& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value, InRhs.Value);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataInt32& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataInt32& InLhs, const FMetadataInt32& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataUInt8& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value), sizeof(InValue.Value));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataUInt8& InLhs, const FMetadataUInt8& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataVector& InLhs, const FMetadataVector& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value.X, InRhs.Value.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Y, InRhs.Value.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Z, InRhs.Value.Z);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataColor& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.DWColor()), sizeof(uint32));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataColor& InLhs, const FMetadataColor& InRhs)
{
	return InLhs.Value.DWColor() == InRhs.Value.DWColor();
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataLinearColor& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.R), sizeof(InValue.Value.R));
	os.write(reinterpret_cast<const char*>(&InValue.Value.G), sizeof(InValue.Value.G));
	os.write(reinterpret_cast<const char*>(&InValue.Value.B), sizeof(InValue.Value.B));
	os.write(reinterpret_cast<const char*>(&InValue.Value.A), sizeof(InValue.Value.A));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataLinearColor& InLhs, const FMetadataLinearColor& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataIntVector& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataIntVector& InLhs, const FMetadataIntVector& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataRotator& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.Pitch), sizeof(InValue.Value.Pitch));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Roll), sizeof(InValue.Value.Roll));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Yaw), sizeof(InValue.Value.Yaw));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataRotator& InLhs, const FMetadataRotator& InRhs)
{
	return InLhs.Value.Equals(InRhs.Value);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataQuat& InValue)
{
	os.write(reinterpret_cast<const char*>(&InValue.Value.X), sizeof(InValue.Value.X));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Y), sizeof(InValue.Value.Y));
	os.write(reinterpret_cast<const char*>(&InValue.Value.Z), sizeof(InValue.Value.Z));
	os.write(reinterpret_cast<const char*>(&InValue.Value.W), sizeof(InValue.Value.W));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataQuat& InLhs, const FMetadataQuat& InRhs)
{
	return InLhs.Value.Equals(InRhs.Value);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataBox& InValue)
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

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataBox& InLhs, const FMetadataBox& InRhs)
{
	return FMath::IsNearlyEqual(InLhs.Value.Min.X, InRhs.Value.Min.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Min.Y, InRhs.Value.Min.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Min.Z, InRhs.Value.Min.Z) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.X, InRhs.Value.Max.X) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.Y, InRhs.Value.Max.Y) &&
		   FMath::IsNearlyEqual(InLhs.Value.Max.Z, InRhs.Value.Max.Z);
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataDateTime& InValue)
{
	const int64 Ticks = InValue.Value.GetTicks();
	os.write(reinterpret_cast<const char*>(&Ticks), sizeof(uint64));
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataDateTime& InLhs, const FMetadataDateTime& InRhs)
{
	return InLhs.Value.GetTicks() == InRhs.Value.GetTicks();
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataString& InValue)
{
	os.write(reinterpret_cast<const char*>(InValue.Value.GetCharArray().GetData()), InValue.Value.GetAllocatedSize());
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataString& InLhs, const FMetadataString& InRhs)
{
	return InLhs.Value == InRhs.Value;
}

FORCEINLINE_DEBUGGABLE std::ostream& operator<<(std::ostream& os, const FMetadataText& InValue)
{
	const FString& SourceString = InValue.Value.ToString();
	os.write(reinterpret_cast<const char*>(SourceString.GetCharArray().GetData()), SourceString.GetAllocatedSize());
	return os;
}

FORCEINLINE_DEBUGGABLE bool operator==(const FMetadataText& InLhs, const FMetadataText& InRhs)
{
	return InLhs.Value.EqualTo(InRhs.Value, ETextComparisonLevel::Default);
}
