#pragma once
//Implementation of 2-component vector. Code copied from IntVector.h and adapted for X,Y

/**
* Structure for integer vectors in 3-d space.
*/
struct FIntVector2
{
	/** Holds the point's x-coordinate. */
	int32 X;

	/** Holds the point's y-coordinate. */
	int32 Y;

public:

	/** An int point with zeroed values. */
	CORE_API static const FIntVector2 ZeroValue;

	/** An int point with INDEX_NONE values. */
	CORE_API static const FIntVector2 NoneValue;

public:

	/**
	* Default constructor (no initialization).
	*/
	FIntVector2();

	/**
	* Creates and initializes a new instance with the specified coordinates.
	*
	* @param InX The x-coordinate.
	* @param InY The y-coordinate.
	* @param InZ The z-coordinate.
	*/
	FIntVector2(int32 InX, int32 InY);

	/**
	* Constructor
	*
	* @param InValue replicated to all components
	*/
	explicit FIntVector2(int32 InValue);

	/**
	* Constructor
	*
	* @param EForceInit Force init enum
	*/
	explicit FORCEINLINE FIntVector2(EForceInit);

public:

	/**
	* Gets specific component of a point.
	*
	* @param ComponentIndex Index of point component.
	* @return const reference to component.
	*/
	const int32& operator()(int32 ComponentIndex) const;

	/**
	* Gets specific component of a point.
	*
	* @param ComponentIndex Index of point component.
	* @return reference to component.
	*/
	int32& operator()(int32 ComponentIndex);

	/**
	* Gets specific component of a point.
	*
	* @param ComponentIndex Index of point component.
	* @return const reference to component.
	*/
	const int32& operator[](int32 ComponentIndex) const;

	/**
	* Gets specific component of a point.
	*
	* @param ComponentIndex Index of point component.
	* @return reference to component.
	*/
	int32& operator[](int32 ComponentIndex);

	/**
	* Compares points for equality.
	*
	* @param Other The other int point being compared.
	* @return true if the points are equal, false otherwise..
	*/
	bool operator==(const FIntVector2& Other) const;

	/**
	* Compares points for inequality.
	*
	* @param Other The other int point being compared.
	* @return true if the points are not equal, false otherwise..
	*/
	bool operator!=(const FIntVector2& Other) const;

	/**
	* Scales this point.
	*
	* @param Scale What to multiply the point by.
	* @return Reference to this point after multiplication.
	*/
	FIntVector2& operator*=(int32 Scale);

	/**
	* Divides this point.
	*
	* @param Divisor What to divide the point by.
	* @return Reference to this point after division.
	*/
	FIntVector2& operator/=(int32 Divisor);

	/**
	* Adds to this point.
	*
	* @param Other The point to add to this point.
	* @return Reference to this point after addition.
	*/
	FIntVector2& operator+=(const FIntVector2& Other);

	/**
	* Subtracts from this point.
	*
	* @param Other The point to subtract from this point.
	* @return Reference to this point after subtraction.
	*/
	FIntVector2& operator-=(const FIntVector2& Other);

	/**
	* Assigns another point to this one.
	*
	* @param Other The point to assign this point from.
	* @return Reference to this point after assignment.
	*/
	FIntVector2& operator=(const FIntVector2& Other);

	/**
	* Gets the result of scaling on this point.
	*
	* @param Scale What to multiply the point by.
	* @return A new scaled int point.
	*/
	FIntVector2 operator*(int32 Scale) const;

	/**
	* Gets the result of division on this point.
	*
	* @param Divisor What to divide the point by.
	* @return A new divided int point.
	*/
	FIntVector2 operator/(int32 Divisor) const;

	/**
	* Gets the result of addition on this point.
	*
	* @param Other The other point to add to this.
	* @return A new combined int point.
	*/
	FIntVector2 operator+(const FIntVector2& Other) const;

	/**
	* Gets the result of subtraction from this point.
	*
	* @param Other The other point to subtract from this.
	* @return A new subtracted int point.
	*/
	FIntVector2 operator-(const FIntVector2& Other) const;

public:

	/**
	* Gets the maximum value in the point.
	*
	* @return The maximum value in the point.
	*/
	float GetMax() const;

	/**
	* Gets the minimum value in the point.
	*
	* @return The minimum value in the point.
	*/
	float GetMin() const;

	/**
	* Gets the distance of this point from (0,0).
	*
	* @return The distance of this point from (0,0).
	*/
	int32 Size() const;

	/**
	* Get a textual representation of this vector.
	*
	* @return A string describing the vector.
	*/
	FString ToString() const;

public:

	/**
	* Divide an int point and round up the result.
	*
	* @param lhs The int point being divided.
	* @param Divisor What to divide the int point by.
	* @return A new divided int point.
	*/
	static FIntVector2 DivideAndRoundUp(FIntVector2 lhs, int32 Divisor);

	/**
	* Gets the number of components a point has.
	*
	* @return Number of components point has.
	*/
	static int32 Num();

public:

	/**
	* Serializes the Rectangle.
	*
	* @param Ar The archive to serialize into.
	* @param Vector The vector to serialize.
	* @return Reference to the Archive after serialization.
	*/
	friend FArchive& operator<<(FArchive& Ar, FIntVector2& Vector)
	{
		return Ar << Vector.X << Vector.Y;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
};


/* FIntVector2 inline functions
*****************************************************************************/

FORCEINLINE FIntVector2::FIntVector2()
{ }


FORCEINLINE FIntVector2::FIntVector2(int32 InX, int32 InY)
	: X(InX)
	, Y(InY)
{ }


FORCEINLINE FIntVector2::FIntVector2(int32 InValue)
	: X(InValue)
	, Y(InValue)
{ }


FORCEINLINE FIntVector2::FIntVector2(EForceInit)
	: X(0)
	, Y(0)
{ }


FORCEINLINE const int32& FIntVector2::operator()(int32 ComponentIndex) const
{
	return (&X)[ComponentIndex];
}


FORCEINLINE int32& FIntVector2::operator()(int32 ComponentIndex)
{
	return (&X)[ComponentIndex];
}


FORCEINLINE const int32& FIntVector2::operator[](int32 ComponentIndex) const
{
	return (&X)[ComponentIndex];
}


FORCEINLINE int32& FIntVector2::operator[](int32 ComponentIndex)
{
	return (&X)[ComponentIndex];
}

FORCEINLINE bool FIntVector2::operator==(const FIntVector2& Other) const
{
	return X == Other.X && Y == Other.Y;
}


FORCEINLINE bool FIntVector2::operator!=(const FIntVector2& Other) const
{
	return X != Other.X || Y != Other.Y;
}


FORCEINLINE FIntVector2& FIntVector2::operator*=(int32 Scale)
{
	X *= Scale;
	Y *= Scale;

	return *this;
}


FORCEINLINE FIntVector2& FIntVector2::operator/=(int32 Divisor)
{
	X /= Divisor;
	Y /= Divisor;

	return *this;
}


FORCEINLINE FIntVector2& FIntVector2::operator+=(const FIntVector2& Other)
{
	X += Other.X;
	Y += Other.Y;

	return *this;
}


FORCEINLINE FIntVector2& FIntVector2::operator-=(const FIntVector2& Other)
{
	X -= Other.X;
	Y -= Other.Y;

	return *this;
}


FORCEINLINE FIntVector2& FIntVector2::operator=(const FIntVector2& Other)
{
	X = Other.X;
	Y = Other.Y;

	return *this;
}


FORCEINLINE FIntVector2 FIntVector2::operator*(int32 Scale) const
{
	return FIntVector2(*this) *= Scale;
}


FORCEINLINE FIntVector2 FIntVector2::operator/(int32 Divisor) const
{
	return FIntVector2(*this) /= Divisor;
}


FORCEINLINE FIntVector2 FIntVector2::operator+(const FIntVector2& Other) const
{
	return FIntVector2(*this) += Other;
}

FORCEINLINE FIntVector2 FIntVector2::operator-(const FIntVector2& Other) const
{
	return FIntVector2(*this) -= Other;
}


FORCEINLINE FIntVector2 FIntVector2::DivideAndRoundUp(FIntVector2 lhs, int32 Divisor)
{
	return FIntVector2(FMath::DivideAndRoundUp(lhs.X, Divisor), FMath::DivideAndRoundUp(lhs.Y, Divisor));
}


FORCEINLINE float FIntVector2::GetMax() const
{
	return FMath::Max(X, Y);
}


FORCEINLINE float FIntVector2::GetMin() const
{
	return FMath::Min(X, Y);
}


FORCEINLINE int32 FIntVector2::Num()
{
	return 2;
}


FORCEINLINE int32 FIntVector2::Size() const
{
	int64 X64 = (int64)X;
	int64 Y64 = (int64)Y;
	return int32(FMath::Sqrt(float(X64 * X64 + Y64 * Y64)));
}


FORCEINLINE FString FIntVector2::ToString() const
{
	return FString::Printf(TEXT("X=%d Y=%d"), X, Y);
}

FORCEINLINE uint32 GetTypeHash(const FIntVector2& Vector)
{
	return FCrc::MemCrc_DEPRECATED(&Vector, sizeof(FIntVector2));
}

template <> struct TIsPODType<FIntVector2> { enum { Value = true }; };
