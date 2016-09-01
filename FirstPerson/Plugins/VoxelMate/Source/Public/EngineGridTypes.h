#pragma once
#include "Platform.h"
#include "Math/Vector2D.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/IntVector.h"
#include "IntVector2.h"
#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>

//The following types are supported as grid voxel types:
//FVector2D, FVector, FVector4, FIntVector, FIntVector4, FUintVector4, FTransform2D,
//FTransform, FRotator, FQuat2D, FQuat, FMatrix2x2, FMatrix, FBox, FBox2D

template<> inline FVector2D openvdb::zeroVal<FVector2D>()
{
	return FVector2D::ZeroVector;
}

template<> inline FVector openvdb::zeroVal<FVector>()
{
	return FVector::ZeroVector;
}

template<> inline FVector4 openvdb::zeroVal<FVector4>()
{
	return FVector4(ForceInitToZero);
}

template<> inline FIntVector2 openvdb::zeroVal<FIntVector2>()
{
	return FIntVector2::ZeroValue;
}

template<> inline FIntVector openvdb::zeroVal<FIntVector>()
{
	return FIntVector::ZeroValue;
}

template<> inline FIntVector4 openvdb::zeroVal<FIntVector4>()
{
	return FIntVector4(ForceInitToZero);
}

template<> inline FUintVector4 openvdb::zeroVal<FUintVector4>()
{
	return FUintVector4(ForceInitToZero);
}

inline FVector2D Abs(const FVector2D& Vec);
std::ostream& operator<<(std::ostream& os, const FVector2D& Vec);

inline FVector Abs(const FVector &Vec);
std::ostream& operator<<(std::ostream& os, const FVector& Vec);
bool operator<(const FVector& Lhs, const FVector& Rhs);
bool operator>(const FVector& Lhs, const FVector& Rhs);

inline FVector4 Abs(const FVector4 &Vec);
std::ostream& operator<<(std::ostream& os, const FVector4& Vec);
FVector4 operator+(const FVector4& Vec, const float& Val);
bool operator<(const FVector4& Lhs, const FVector4& Rhs);
bool operator>(const FVector4& Lhs, const FVector4& Rhs);

inline FIntVector2 Abs(const FIntVector2 &IVec);
std::ostream& operator<<(std::ostream& os, const FIntVector2& Vec);
FIntVector2 operator+(const FIntVector2& Vec, const float& Val);
FIntVector2 operator-(const FIntVector2& Vec);
bool operator<(const FIntVector2& Lhs, const FIntVector2& Rhs);
bool operator>(const FIntVector2& Lhs, const FIntVector2& Rhs);

inline FIntVector Abs(const FIntVector &IVec);
std::ostream& operator<<(std::ostream& os, const FIntVector& Vec);
FIntVector operator+(const FIntVector& Vec, const float& Val);
FIntVector operator-(const FIntVector& Vec);
bool operator<(const FIntVector& Lhs, const FIntVector& Rhs);
bool operator>(const FIntVector& Lhs, const FIntVector& Rhs);

inline FIntVector4 Abs(const FIntVector4 &IVec);
std::ostream& operator<<(std::ostream& os, const FIntVector4& Vec);
FIntVector4 operator+(const FIntVector4& Vec, const float& Val);
FIntVector4 operator+(const FIntVector4& Lhs, const FIntVector4& Rhs);
FIntVector4 operator-(const FIntVector4& Lhs, const FIntVector4& Rhs);
FIntVector4 operator-(const FIntVector4& Vec);
bool operator<(const FIntVector4& Lhs, const FIntVector4& Rhs);
bool operator>(const FIntVector4& Lhs, const FIntVector4& Rhs);

inline FUintVector4 Abs(const FUintVector4 &UVec);
std::ostream& operator<<(std::ostream& os, const FUintVector4& Vec);
FUintVector4 operator+(const FUintVector4& Vec, const float& Val);
FUintVector4 operator+(const FUintVector4& Lhs, const FUintVector4& Rhs);
FUintVector4 operator-(const FUintVector4& Lhs, const FUintVector4& Rhs);
FUintVector4 operator-(const FUintVector4& Vec);
bool operator<(const FUintVector4& Lhs, const FUintVector4& Rhs);
bool operator>(const FUintVector4& Lhs, const FUintVector4& Rhs);
