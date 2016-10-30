#include "VoxelMatePrivatePCH.h"
#include "EngineGridTypes.h"

FVector2D Abs(const FVector2D &Vec)
{
	return Vec.GetAbs();
}

std::ostream& operator<<(std::ostream& os, const FVector2D& Vec)
{
	os << Vec.X << Vec.Y;
	return os;
}

FVector Abs(const FVector &Vec)
{
	return Vec.GetAbs();
}

std::ostream& operator<<(std::ostream& os, const FVector& Vec)
{
	os << Vec.X << Vec.Y << Vec.Z;
	return os;
}

bool operator<(const FVector& Lhs, const FVector& Rhs)
{
	return Lhs.X < Rhs.X ||
           Lhs.Y < Rhs.Y ||
           Lhs.Z < Rhs.Z;
}

bool operator>(const FVector& Lhs, const FVector& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y &&
		   Lhs.Z > Rhs.Z;
}

FVector4 Abs(const FVector4 &Vec)
{
	return FVector4(FMath::Abs(Vec.X), FMath::Abs(Vec.Y), FMath::Abs(Vec.Z), FMath::Abs(Vec.W));
}

std::ostream& operator<<(std::ostream& os, const FVector4& Vec)
{
	os << Vec.X << Vec.Y << Vec.Z << Vec.W;
	return os;
}

FVector4 operator+(const FVector4& Vec, const float& Val)
{
	return FVector4(Vec.X + Val, Vec.Y + Val, Vec.Z + Val, Vec.W + Val);
}

bool operator<(const FVector4& Lhs, const FVector4& Rhs)
{
	return Lhs.X < Rhs.X ||
		   Lhs.Y < Rhs.Y ||
		   Lhs.Z < Rhs.Z ||
		   Lhs.W < Rhs.W;
}

bool operator>(const FVector4& Lhs, const FVector4& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y &&
		   Lhs.Z > Rhs.Z &&
		   Lhs.W > Rhs.W;
}

FIntVector2 Abs(const FIntVector2 &IVec)
{
	return FIntVector2(FMath::Abs(IVec.X), FMath::Abs(IVec.Y));
}

std::ostream& operator<<(std::ostream& os, const FIntVector2& Vec)
{
	os << Vec.X << Vec.Y;
	return os;
}

FIntVector2 operator+(const FIntVector2& Vec, const float& Val)
{
	return FIntVector2(Vec.X + Val, Vec.Y + Val);
}

FIntVector2 operator-(const FIntVector2& Vec)
{
	return FIntVector2(-Vec.X, -Vec.Y);
}

bool operator<(const FIntVector2& Lhs, const FIntVector2& Rhs)
{
	return Lhs.X < Rhs.X ||
		   Lhs.Y < Rhs.Y;
}

bool operator>(const FIntVector2& Lhs, const FIntVector2& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y;
}

FIntVector Abs(const FIntVector &IVec)
{
	return FIntVector(FMath::Abs(IVec.X), FMath::Abs(IVec.Y), FMath::Abs(IVec.Z));
}

std::ostream& operator<<(std::ostream& os, const FIntVector& Vec)
{
	os << Vec.X << Vec.Y << Vec.Z;
	return os;
}

FIntVector operator+(const FIntVector& Vec, const float& Val)
{
	return FIntVector(Vec.X + Val, Vec.Y + Val, Vec.Z + Val);
}

FIntVector operator-(const FIntVector& Vec)
{
	return FIntVector(-Vec.X, -Vec.Y, -Vec.Z);
}

bool operator<(const FIntVector& Lhs, const FIntVector& Rhs)
{
	return Lhs.X < Rhs.X ||
		   Lhs.Y < Rhs.Y ||
		   Lhs.Z < Rhs.Z;
}

bool operator>(const FIntVector& Lhs, const FIntVector& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y &&
		   Lhs.Z > Rhs.Z;
}

FIntVector4 Abs(const FIntVector4 &IVec)
{
	return FIntVector4(FMath::Abs(IVec.X), FMath::Abs(IVec.Y), FMath::Abs(IVec.Z), FMath::Abs(IVec.W));
}

std::ostream& operator<<(std::ostream& os, const FIntVector4& Vec)
{
	os << Vec.X << Vec.Y << Vec.Z << Vec.W;
	return os;
}

FIntVector4 operator+(const FIntVector4& Vec, const float& Val)
{
	return FIntVector4(Vec.X + Val, Vec.Y + Val, Vec.Z + Val, Vec.W + Val);
}

FIntVector4 operator+(const FIntVector4& Lhs, const FIntVector4& Rhs)
{
	return FIntVector4(Lhs.X + Rhs.X, Lhs.Y + Rhs.Y, Lhs.Z + Rhs.Z, Lhs.W + Rhs.W);
}

FIntVector4 operator-(const FIntVector4& Lhs, const FIntVector4& Rhs)
{
	return FIntVector4(Lhs.X - Rhs.X, Lhs.Y - Rhs.Y, Lhs.Z - Rhs.Z, Lhs.W - Rhs.W);
}

FIntVector4 operator-(const FIntVector4& Vec)
{
	return FIntVector4(-Vec.X, -Vec.Y, -Vec.Z, -Vec.W);
}

bool operator<(const FIntVector4& Lhs, const FIntVector4& Rhs)
{
	return Lhs.X < Rhs.X ||
		   Lhs.Y < Rhs.Y ||
		   Lhs.Z < Rhs.Z ||
		   Lhs.W < Rhs.W;
}

bool operator>(const FIntVector4& Lhs, const FIntVector4& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y &&
		   Lhs.Z > Rhs.Z &&
		   Lhs.W > Rhs.W;
}

FUintVector4 Abs(const FUintVector4 &UVec)
{
	return UVec; //unsigned type - don't need to do anything
}

std::ostream& operator<<(std::ostream& os, const FUintVector4& Vec)
{
	os << Vec.X << Vec.Y << Vec.Z << Vec.W;
	return os;
}

FUintVector4 operator+(const FUintVector4& Vec, const float& Val)
{
	return FUintVector4(Vec.X + Val, Vec.Y + Val, Vec.Z + Val, Vec.W + Val);
}

FUintVector4 operator+(const FUintVector4& Lhs, const FUintVector4& Rhs)
{
	return FUintVector4(Lhs.X + Rhs.X, Lhs.Y + Rhs.Y, Lhs.Z + Rhs.Z, Lhs.W + Rhs.W);
}

FUintVector4 operator-(const FUintVector4& Lhs, const FUintVector4& Rhs)
{
	return FUintVector4(Lhs.X - Rhs.X, Lhs.Y - Rhs.Y, Lhs.Z - Rhs.Z, Lhs.W - Rhs.W);
}

FUintVector4 operator-(const FUintVector4& Vec)
{
	return FUintVector4((uint32)(-(int32)Vec.X), (uint32)(-(int32)Vec.Y), (uint32)(-(int32)Vec.Z), (uint32)(-(int32)Vec.W));
}

bool operator<(const FUintVector4& Lhs, const FUintVector4& Rhs)
{
	return Lhs.X < Rhs.X ||
		   Lhs.Y < Rhs.Y ||
		   Lhs.Z < Rhs.Z ||
		   Lhs.W < Rhs.W;
}

bool operator>(const FUintVector4& Lhs, const FUintVector4& Rhs)
{
	return Lhs.X > Rhs.X &&
		   Lhs.Y > Rhs.Y &&
		   Lhs.Z > Rhs.Z &&
		   Lhs.W > Rhs.W;
}

std::ostream& operator<<(std::ostream& os, const FMatrix& Mat)
{
	os << Mat.M[0][0] << Mat.M[0][1] << Mat.M[0][2] << Mat.M[0][3];
	os << Mat.M[1][0] << Mat.M[1][1] << Mat.M[1][2] << Mat.M[1][3];
	os << Mat.M[2][0] << Mat.M[2][1] << Mat.M[2][2] << Mat.M[2][3];
	os << Mat.M[3][0] << Mat.M[3][1] << Mat.M[3][2] << Mat.M[3][3];
	return os;
}

std::ostream& operator<<(std::ostream& os, const FString& Str)
{
    os << *Str;
    return os;
}
