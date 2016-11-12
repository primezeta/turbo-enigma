#pragma once
#include "Platform.h"
#include "EngineMinimal.h"
#include "VoxelData.h"
#include "GridTransformMaps.h"
#include "VoxelDatabaseCommon.h"
#include "VoxelDatabaseTypeFactory.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <ostream>

typedef std::ostream OutputStreamType;

namespace HasOperator_Private
{
    struct FNotSpecified {};

    template <typename T>
    struct FReturnValueCheck
    {
        static char(&Func())[2];
    };

    template <>
    struct FReturnValueCheck<FNotSpecified>
    {
        static char(&Func())[1];
    };

    template <typename T>
    FNotSpecified operator+(const T&, const T&);

    template <typename T>
    FNotSpecified operator+(const T&, float);

    template <typename T>
    FNotSpecified operator-(const T&, const T&);

    template <typename T>
    FNotSpecified operator<(const T&, const T&);

    template <typename T>
    FNotSpecified operator>(const T&, const T&);

    template <typename T>
    const T& Make();

    template <typename T>
    struct Plus
    {
        enum { Value = sizeof(FReturnValueCheck<decltype(Make<T>() + Make<T>())>::Func()) == sizeof(char[2]) };
    };

    template <typename T>
    struct PlusFloat
    {
        enum { Value = sizeof(FReturnValueCheck<decltype(Make<T>() + Make<float>())>::Func()) == sizeof(char[2]) };
    };

    template <typename T>
    struct Minus
    {
        enum { Value = sizeof(FReturnValueCheck<decltype(Make<T>() - Make<T>())>::Func()) == sizeof(char[2]) };
    };

    template <typename T>
    struct LessThan
    {
        enum { Value = sizeof(FReturnValueCheck<decltype(Make<T>() < Make<T>())>::Func()) == sizeof(char[2]) };
    };

    template <typename T>
    struct GreaterThan
    {
        enum { Value = sizeof(FReturnValueCheck<decltype(Make<T>() > Make<T>())>::Func()) == sizeof(char[2]) };
    };
}

/**
* Traits class which tests if a type has an operator+ overload.
*/
template <typename T>
struct THasOperatorPlus
{
    enum { Value = HasOperator_Private::Plus<T>::Value };
};

/**
* Traits class which tests if a type has an operator+ overload.
*/
template <typename T>
struct THasOperatorPlusFloat
{
    enum { Value = HasOperator_Private::PlusFloat<T>::Value };
};

/**
* Traits class which tests if a type has an operator- overload.
*/
template <typename T>
struct THasOperatorMinus
{
    enum { Value = HasOperator_Private::Minus<T>::Value };
};

/**
* Traits class which tests if a type has an operator< overload.
*/
template <typename T>
struct THasOperatorLessThan
{
    enum { Value = HasOperator_Private::LessThan<T>::Value };
};

/**
* Traits class which tests if a type has an operator> overload.
*/
template <typename T>
struct THasOperatorGreaterThan
{
    enum { Value = HasOperator_Private::GreaterThan<T>::Value };
};

template<typename T>
struct TIsZeroConstructNonPointerType
{
    enum { Value = TIsEnum<T>::Value || TIsArithmetic<T>::Value };
};

template<typename T>
struct TIsNonZeroConstructAndNonPointerType
{
    enum { Value = !TIsZeroConstructNonPointerType<T>::Value && TIsPointer<T>::Value };
};

template<typename ZeroConstructType>
FORCEINLINE typename TEnableIf<TIsZeroConstructNonPointerType<ZeroConstructType>::Value, FPlatformTypes::ANSICHAR*>::Type AsDataBytes(ZeroConstructType* Value)
{
    return reinterpret_cast<FPlatformTypes::ANSICHAR*>(Value);
}

template<typename ZeroConstructType>
FORCEINLINE typename TEnableIf<TIsZeroConstructNonPointerType<ZeroConstructType>::Value, const FPlatformTypes::ANSICHAR*>::Type AsDataBytes(const ZeroConstructType* Value)
{
    return reinterpret_cast<const FPlatformTypes::ANSICHAR*>(Value);
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<TIsZeroConstructNonPointerType<Ue4Type>::Value, void>::Type
WriteValue(OutputStreamType& os, const Ue4Type& InValue)
{
    const FPlatformTypes::ANSICHAR* Bytes = AsDataBytes<Ue4Type>(&InValue);
    const uint32 Count = sizeof(Ue4Type);
    os.write(Bytes, Count);
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsZeroConstructNonPointerType<Ue4Type>::Value, void>::Type
WriteValue(OutputStreamType& os, const Ue4Type& InValue)
{
    static_assert(false, "WriteValue not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector>::Value, void>::Type
WriteValue<FVector>(OutputStreamType& os, const FVector& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector4>::Value, void>::Type
WriteValue(OutputStreamType& os, const FVector4& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector2D>::Value, void>::Type
WriteValue(OutputStreamType& os, const FVector2D& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FColor>::Value, void>::Type
WriteValue(OutputStreamType& os, const FColor& InValue)
{
    WriteValue<uint8>(os, InValue.R);
    WriteValue<uint8>(os, InValue.G);
    WriteValue<uint8>(os, InValue.B);
    WriteValue<uint8>(os, InValue.A);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FLinearColor>::Value, void>::Type
WriteValue(OutputStreamType& os, const FLinearColor& InValue)
{
    WriteValue<float>(os, InValue.R);
    WriteValue<float>(os, InValue.G);
    WriteValue<float>(os, InValue.B);
    WriteValue<float>(os, InValue.A);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedNormal>::Value, void>::Type
WriteValue(OutputStreamType& os, const FPackedNormal& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedRGB10A2N>::Value, void>::Type
WriteValue(OutputStreamType& os, const FPackedRGB10A2N& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedRGBA16N>::Value, void>::Type
WriteValue(OutputStreamType& os, const FPackedRGBA16N& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FIntPoint>::Value, void>::Type
WriteValue(OutputStreamType& os, const FIntPoint& InValue)
{
    WriteValue<int32>(os, InValue.X);
    WriteValue<int32>(os, InValue.Y);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FIntVector>::Value, void>::Type
WriteValue(OutputStreamType& os, const FIntVector& InValue)
{
    WriteValue<int32>(os, InValue.X);
    WriteValue<int32>(os, InValue.Y);
    WriteValue<int32>(os, InValue.Z);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatRangeBound>::Value, void>::Type
WriteValue(OutputStreamType& os, const FFloatRangeBound& InValue)
{
    if (InValue.IsExclusive())
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Exclusive);
        WriteValue<float>(os, InValue.GetValue());
    }
    else if (InValue.IsInclusive())
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Inclusive);
        WriteValue<float>(os, InValue.GetValue());
    }
    else //IsOpen
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Open);
    }
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatRange>::Value, void>::Type
WriteValue(OutputStreamType& os, const FFloatRange& InValue)
{
    WriteValue<FFloatRangeBound>(os, InValue.GetLowerBoundValue());
    WriteValue<FFloatRangeBound>(os, InValue.GetUpperBoundValue());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32RangeBound>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInt32RangeBound& InValue)
{
    if (InValue.IsExclusive())
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Exclusive);
        WriteValue<int32>(os, InValue.GetValue());
    }
    else if (InValue.IsInclusive())
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Inclusive);
        WriteValue<int32>(os, InValue.GetValue());
    }
    else //IsOpen
    {
        WriteValue<uint8>(os, ERangeBoundTypes::Open);
    }
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32Range>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInt32Range& InValue)
{
    WriteValue<FInt32RangeBound>(os, InValue.GetLowerBoundValue());
    WriteValue<FInt32RangeBound>(os, InValue.GetUpperBoundValue());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatInterval>::Value, void>::Type
WriteValue(OutputStreamType& os, const FFloatInterval& InValue)
{
    WriteValue<float>(os, InValue.Min);
    WriteValue<float>(os, InValue.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32Interval>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInt32Interval& InValue)
{
    WriteValue<int32>(os, InValue.Min);
    WriteValue<int32>(os, InValue.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTwoVectors>::Value, void>::Type
WriteValue(OutputStreamType& os, const FTwoVectors& InValue)
{
    WriteValue<FVector>(os, InValue.v1);
    WriteValue<FVector>(os, InValue.v2);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPlane>::Value, void>::Type
WriteValue(OutputStreamType& os, const FPlane& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FRotator>::Value, void>::Type
WriteValue(OutputStreamType& os, const FRotator& InValue)
{
    WriteValue<float>(os, InValue.Pitch);
    WriteValue<float>(os, InValue.Roll);
    WriteValue<float>(os, InValue.Yaw);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FQuat>::Value, void>::Type
WriteValue(OutputStreamType& os, const FQuat& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBox>::Value, void>::Type
WriteValue(OutputStreamType& os, const FBox& InValue)
{
    WriteValue<FVector>(os, InValue.Min);
    WriteValue<FVector>(os, InValue.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBox2D>::Value, void>::Type
WriteValue(OutputStreamType& os, const FBox2D& InValue)
{
    WriteValue<FVector2D>(os, InValue.Min);
    WriteValue<FVector2D>(os, InValue.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBoxSphereBounds>::Value, void>::Type
WriteValue(OutputStreamType& os, const FBoxSphereBounds& InValue)
{
    WriteValue<FVector>(os, InValue.Origin);
    WriteValue<FVector>(os, InValue.BoxExtent);
    WriteValue<float>(os, InValue.SphereRadius);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FOrientedBox>::Value, void>::Type
WriteValue(OutputStreamType& os, const FOrientedBox& InValue)
{
    WriteValue<FVector>(os, InValue.Center);
    WriteValue<FVector>(os, InValue.AxisX);
    WriteValue<FVector>(os, InValue.AxisY);
    WriteValue<FVector>(os, InValue.AxisZ);
    WriteValue<float>(os, InValue.ExtentX);
    WriteValue<float>(os, InValue.ExtentY);
    WriteValue<float>(os, InValue.ExtentZ);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FMatrix>::Value, void>::Type
WriteValue(OutputStreamType& os, const FMatrix& InValue)
{
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(0));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(1));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(2));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(3));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointFloat>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointFloat& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<float>(os, InValue.OutVal);
    WriteValue<float>(os, InValue.ArriveTangent);
    WriteValue<float>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveFloat>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveFloat& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointFloat>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointVector2D>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointVector2D& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FVector2D>(os, InValue.OutVal);
    WriteValue<FVector2D>(os, InValue.ArriveTangent);
    WriteValue<FVector2D>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveVector2D>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveVector2D& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointVector2D>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointVector>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointVector& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FVector>(os, InValue.OutVal);
    WriteValue<FVector>(os, InValue.ArriveTangent);
    WriteValue<FVector>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveVector>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveVector& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointVector>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointQuat>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointQuat& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FQuat>(os, InValue.OutVal);
    WriteValue<FQuat>(os, InValue.ArriveTangent);
    WriteValue<FQuat>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveQuat>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveQuat& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointQuat>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointTwoVectors>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointTwoVectors& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FTwoVectors>(os, InValue.OutVal);
    WriteValue<FTwoVectors>(os, InValue.ArriveTangent);
    WriteValue<FTwoVectors>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveTwoVectors>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveTwoVectors& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointTwoVectors>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointLinearColor>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurvePointLinearColor& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FLinearColor>(os, InValue.OutVal);
    WriteValue<FLinearColor>(os, InValue.ArriveTangent);
    WriteValue<FLinearColor>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveLinearColor>::Value, void>::Type
WriteValue(OutputStreamType& os, const FInterpCurveLinearColor& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointLinearColor>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTransform>::Value, void>::Type
WriteValue(OutputStreamType& os, const FTransform& InValue)
{
    WriteValue<FQuat>(os, InValue.GetRotation());
    WriteValue<FVector>(os, InValue.GetTranslation());
    WriteValue<FVector>(os, InValue.GetScale3D());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FRandomStream>::Value, void>::Type
WriteValue(OutputStreamType& os, const FRandomStream& InValue)
{
    WriteValue<int32>(os, InValue.GetCurrentSeed());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FDateTime>::Value, void>::Type
WriteValue(OutputStreamType& os, const FDateTime& InValue)
{
    WriteValue<int64>(os, InValue.GetTicks());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTimespan>::Value, void>::Type
WriteValue(OutputStreamType& os, const FTimespan& InValue)
{
    WriteValue<int64>(os, InValue.GetTicks());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FString>::Value, void>::Type
WriteValue(OutputStreamType& os, const FString& InValue)
{
    if (InValue.Len() > 0)
    {
        const TCHAR* Str = (const TCHAR*)&InValue.GetCharArray();
        const ANSICHAR* DataBytes = TCHAR_TO_ANSI(Str);
        const int32 DataSize = TCString<ANSICHAR>::Strlen(DataBytes);
        os.write(DataBytes, DataSize);
    }
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FStringAssetReference>::Value, void>::Type
WriteValue(OutputStreamType& os, const FStringAssetReference& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FStringClassReference>::Value, void>::Type
WriteValue(OutputStreamType& os, const FStringClassReference& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FName>::Value, void>::Type
WriteValue(OutputStreamType& os, const FName& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FText>::Value, void>::Type
WriteValue(OutputStreamType& os, const FText& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FAffineMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FAffineMap& InValue)
{
    WriteValue<FMatrix>(os, static_cast<FMatrix>(InValue));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUnitaryMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FUnitaryMap& InValue)
{
    WriteValue<FVector>(os, InValue.GetRotationAxis());
    WriteValue<float>(os, 2.f * FMath::Acos(InValue.W));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FScaleMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FScaleMap& InValue)
{
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(0)[0]);
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(1)[1]);
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(2)[2]);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUniformScaleMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FUniformScaleMap& InValue)
{
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(0)[0]);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTranslationMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FTranslationMap& InValue)
{
    WriteValue<FVector>(os, InValue.FTranslationMatrix::GetColumn(0));
    WriteValue<FVector>(os, InValue.FTranslationMatrix::GetColumn(1));
    WriteValue<FVector>(os, InValue.FTranslationMatrix::GetColumn(2));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FScaleTranslationMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FScaleTranslationMap& InValue)
{
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(0)[0]);
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(1)[1]);
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(2)[2]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(0)[3]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(1)[3]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(2)[3]);
    WriteValue<FVector>(os, InValue.FTranslationMatrix::GetOrigin());//TODO: Confirm that GetOrigin returns the translation
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUniformScaleTranslationMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FUniformScaleTranslationMap& InValue)
{
    WriteValue<float>(os, InValue.FScaleMatrix::GetColumn(0)[0]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(0)[3]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(1)[3]);
    //WriteValue<float>(os, InValue.FTranslationMatrix::GetColumn(2)[3]);
    WriteValue<FVector>(os, InValue.FTranslationMatrix::GetOrigin());//TODO: Confirm that GetOrigin returns the translation
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FNonlinearFrustumMap>::Value, void>::Type
WriteValue(OutputStreamType& os, const FNonlinearFrustumMap& InValue)
{
    WriteValue<float>(os, InValue.Taper);
    WriteValue<float>(os, InValue.Depth);
    WriteValue<FVector>(os, InValue.Min);
    WriteValue<FVector>(os, InValue.Max);
    WriteValue<uint8>(os, InValue.IsValid);
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsZeroConstructNonPointerType<Ue4Type>::Value, bool>::Type
AreValuesEqual(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "AreValuesEqual not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector>::Value, bool>::Type
AreValuesEqual(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector4>::Value, bool>::Type
AreValuesEqual(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FVector2D>::Value, bool>::Type
AreValuesEqual(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FColor>::Value, bool>::Type
AreValuesEqual(const FColor& InLhs, const FColor& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FLinearColor>::Value, bool>::Type
AreValuesEqual(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedNormal>::Value, bool>::Type
AreValuesEqual(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedRGB10A2N>::Value, bool>::Type
AreValuesEqual(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPackedRGBA16N>::Value, bool>::Type
AreValuesEqual(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FIntPoint>::Value, bool>::Type
AreValuesEqual(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FIntVector>::Value, bool>::Type
AreValuesEqual(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatRange>::Value, bool>::Type
AreValuesEqual(const FFloatRange& InLhs, const FFloatRange& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32Range>::Value, bool>::Type
AreValuesEqual(const FInt32Range& InLhs, const FInt32Range& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatInterval>::Value, bool>::Type
AreValuesEqual(const FFloatInterval& InLhs, const FFloatInterval& InRhs)
{
    return FMath::IsNearlyEqual(InLhs.Min, InRhs.Min) && FMath::IsNearlyEqual(InLhs.Max, InRhs.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32Interval>::Value, bool>::Type
AreValuesEqual(const FInt32Interval& InLhs, const FInt32Interval& InRhs)
{
    return InLhs.Min == InRhs.Min && InLhs.Max == InRhs.Max;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FFloatRangeBound>::Value, bool>::Type
AreValuesEqual(const FFloatRangeBound& InLhs, const FFloatRangeBound& InRhs)
{
    return (InLhs.IsOpen() && InRhs.IsOpen()) ||
           (!InLhs.IsOpen() && !InRhs.IsOpen() && InLhs.GetValue() == InRhs.GetValue());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInt32RangeBound>::Value, bool>::Type
AreValuesEqual(const FInt32RangeBound& InLhs, const FInt32RangeBound& InRhs)
{
    return (InLhs.IsOpen() && InRhs.IsOpen()) ||
           (!InLhs.IsOpen() && !InRhs.IsOpen() && InLhs.GetValue() == InRhs.GetValue());
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTwoVectors>::Value, bool>::Type
AreValuesEqual(const FTwoVectors& InLhs, const FTwoVectors& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FPlane>::Value, bool>::Type
AreValuesEqual(const FPlane& InLhs, const FPlane& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FRotator>::Value, bool>::Type
AreValuesEqual(const FRotator& InLhs, const FRotator& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FQuat>::Value, bool>::Type
AreValuesEqual(const FQuat& InLhs, const FQuat& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBox>::Value, bool>::Type
AreValuesEqual(const FBox& InLhs, const FBox& InRhs)
{
    return InLhs.Min.Equals(InRhs.Min) && InLhs.Max.Equals(InRhs.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBox2D>::Value, bool>::Type
AreValuesEqual(const FBox2D& InLhs, const FBox2D& InRhs)
{
    return InLhs.Min.Equals(InRhs.Min) && InLhs.Max.Equals(InRhs.Max);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FBoxSphereBounds>::Value, bool>::Type
AreValuesEqual(const FBoxSphereBounds& InLhs, const FBoxSphereBounds& InRhs)
{
    return InLhs.Origin.Equals(InRhs.Origin) &&
           InLhs.BoxExtent.Equals(InRhs.BoxExtent) &&
           FMath::IsNearlyEqual(InLhs.SphereRadius, InRhs.SphereRadius);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FOrientedBox>::Value, bool>::Type
AreValuesEqual(const FOrientedBox& InLhs, const FOrientedBox& InRhs)
{
    return InLhs.Center.Equals(InRhs.Center) &&
           InLhs.AxisX.Equals(InRhs.AxisX) &&
           InLhs.AxisY.Equals(InRhs.AxisY) &&
           InLhs.AxisZ.Equals(InRhs.AxisZ) &&
           FMath::IsNearlyEqual(InLhs.ExtentX, InRhs.ExtentX) &&
           FMath::IsNearlyEqual(InLhs.ExtentY, InRhs.ExtentY) &&
           FMath::IsNearlyEqual(InLhs.ExtentZ, InRhs.ExtentZ);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FMatrix>::Value, bool>::Type
AreValuesEqual(const FMatrix& InLhs, const FMatrix& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointFloat>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointFloat& InLhs, const FInterpCurvePointFloat& InRhs)
{
    return FMath::IsNearlyEqual(InLhs.ArriveTangent, InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           FMath::IsNearlyEqual(InLhs.OutVal, InRhs.OutVal) &&
           FMath::IsNearlyEqual(InLhs.ArriveTangent, InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.LeaveTangent, InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveFloat>::Value, bool>::Type
AreValuesEqual(const FInterpCurveFloat& InLhs, const FInterpCurveFloat& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointFloat>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointVector2D>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointVector2D& InLhs, const FInterpCurvePointVector2D& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveVector2D>::Value, bool>::Type
AreValuesEqual(const FInterpCurveVector2D& InLhs, const FInterpCurveVector2D& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointVector2D>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointVector>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointVector& InLhs, const FInterpCurvePointVector& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveVector>::Value, bool>::Type
AreValuesEqual(const FInterpCurveVector& InLhs, const FInterpCurveVector& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointVector>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointQuat>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointQuat& InLhs, const FInterpCurvePointQuat& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveQuat>::Value, bool>::Type
AreValuesEqual(const FInterpCurveQuat& InLhs, const FInterpCurveQuat& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointQuat>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointTwoVectors>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointTwoVectors& InLhs, const FInterpCurvePointTwoVectors& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveTwoVectors>::Value, bool>::Type
AreValuesEqual(const FInterpCurveTwoVectors& InLhs, const FInterpCurveTwoVectors& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointTwoVectors>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurvePointLinearColor>::Value, bool>::Type
AreValuesEqual(const FInterpCurvePointLinearColor& InLhs, const FInterpCurvePointLinearColor& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FInterpCurveLinearColor>::Value, bool>::Type
AreValuesEqual(const FInterpCurveLinearColor& InLhs, const FInterpCurveLinearColor& InRhs)
{
    //for (auto i = InLhs.Points.CreateConstIterator(); i; ++i)
    //{
    //    if (!AreValuesEqual<FInterpCurvePointLinearColor>(*i))
    //    {
    //        return false;
    //    }
    //}
    //return InLhs.bIsLooped == InRhs.bIsLooped &&
    //       (!InLhs.bIsLooped || InLhs.LoopKeyOffset == InRhs.LoopKeyOffset);
    //The above could be used to utilize proper float comparisons, but
    //much slower than below which compares the TArray of points using operator==
    //which compares the arrays exactly with Memcmp
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTransform>::Value, bool>::Type
AreValuesEqual(const FTransform& InLhs, const FTransform& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FRandomStream>::Value, bool>::Type
AreValuesEqual(const FRandomStream& InLhs, const FRandomStream& InRhs)
{
    return InLhs.GetCurrentSeed() == InRhs.GetCurrentSeed();
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FDateTime>::Value, bool>::Type
AreValuesEqual(const FDateTime& InLhs, const FDateTime& InRhs)
{
    return InLhs.GetTicks() == InRhs.GetTicks();
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTimespan>::Value, bool>::Type
AreValuesEqual(const FTimespan& InLhs, const FTimespan& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FStringAssetReference>::Value, bool>::Type
AreValuesEqual(const FStringAssetReference& InLhs, const FStringAssetReference& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FStringClassReference>::Value, bool>::Type
AreValuesEqual(const FStringClassReference& InLhs, const FStringClassReference& InRhs)
{
    return InLhs.ToString() == InRhs.ToString();
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FString>::Value, bool>::Type
AreValuesEqual(const FString& InLhs, const FString& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FName>::Value, bool>::Type
AreValuesEqual(const FName& InLhs, const FName& InRhs)
{
    return InLhs == InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FText>::Value, bool>::Type
AreValuesEqual(const FText& InLhs, const FText& InRhs)
{
    return InLhs.EqualTo(InRhs);
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FAffineMap>::Value, bool>::Type
AreValuesEqual(const FAffineMap& InLhs, const FAffineMap& InRhs)
{
    return static_cast<FMatrix>(InLhs).Equals(static_cast<FMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUnitaryMap>::Value, bool>::Type
AreValuesEqual(const FUnitaryMap& InLhs, const FUnitaryMap& InRhs)
{
    return static_cast<FQuat>(InLhs).Equals(static_cast<FQuat>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FScaleMap>::Value, bool>::Type
AreValuesEqual(const FScaleMap& InLhs, const FScaleMap& InRhs)
{
    return static_cast<FScaleMatrix>(InLhs).Equals(static_cast<FScaleMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUniformScaleMap>::Value, bool>::Type
AreValuesEqual(const FUniformScaleMap& InLhs, const FUniformScaleMap& InRhs)
{
    return static_cast<FScaleMatrix>(InLhs).Equals(static_cast<FScaleMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FTranslationMap>::Value, bool>::Type
AreValuesEqual(const FTranslationMap& InLhs, const FTranslationMap& InRhs)
{
    return static_cast<FTranslationMatrix>(InLhs).Equals(static_cast<FTranslationMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FScaleTranslationMap>::Value, bool>::Type
AreValuesEqual(const FScaleTranslationMap& InLhs, const FScaleTranslationMap& InRhs)
{
    return static_cast<FScaleMatrix>(InLhs).Equals(static_cast<FScaleMatrix>(InRhs)) &&
           static_cast<FTranslationMatrix>(InLhs).Equals(static_cast<FTranslationMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FUniformScaleTranslationMap>::Value, bool>::Type
AreValuesEqual(const FUniformScaleTranslationMap& InLhs, const FUniformScaleTranslationMap& InRhs)
{
    return static_cast<FScaleMatrix>(InLhs).Equals(static_cast<FScaleMatrix>(InRhs)) &&
           static_cast<FTranslationMatrix>(InLhs).Equals(static_cast<FTranslationMatrix>(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!TIsNonZeroConstructAndNonPointerType<FNonlinearFrustumMap>::Value, bool>::Type
AreValuesEqual(const FNonlinearFrustumMap& InLhs, const FNonlinearFrustumMap& InRhs)
{
    return AreValuesEqual<FBox>(static_cast<FBox>(InLhs), static_cast<FBox>(InRhs)) &&
           FMath::IsNearlyEqual(InLhs.Taper, InRhs.Taper) &&
           FMath::IsNearlyEqual(InLhs.Depth, InRhs.Depth);
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, bool>::Type
IsValueLessThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueLessThan not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FLinearColor>::Value && !TIsArithmetic<FLinearColor>::Value, bool>::Type
IsValueLessThan(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() < FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FColor>::Value && !TIsArithmetic<FColor>::Value, bool>::Type
IsValueLessThan(const FColor& InLhs, const FColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() < FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FVector>::Value && !TIsArithmetic<FVector>::Value, bool>::Type
IsValueLessThan(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y && InLhs.Z < InRhs.Z;
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<THasOperatorLessThan<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, bool>::Type
IsValueLessThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueLessThan not implemented");
}

template<>
FORCEINLINE typename TEnableIf<THasOperatorLessThan<FVector2D>::Value && !TIsArithmetic<FVector2D>::Value, bool>::Type
IsValueLessThan(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs < InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FVector4>::Value && !TIsArithmetic<FVector4>::Value, bool>::Type
IsValueLessThan(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y && InLhs.Z < InRhs.Z && InLhs.Z < InRhs.Z;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FPackedNormal>::Value && !TIsArithmetic<FPackedNormal>::Value, bool>::Type
IsValueLessThan(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FPackedRGB10A2N>::Value && !TIsArithmetic<FPackedRGB10A2N>::Value, bool>::Type
IsValueLessThan(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorLessThan<FPackedRGBA16N>::Value && !TIsArithmetic<FPackedRGBA16N>::Value, bool>::Type
IsValueLessThan(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, bool>::Type
IsValueGreaterThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueGreaterThan not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FLinearColor>::Value && !TIsArithmetic<FLinearColor>::Value, bool>::Type
IsValueGreaterThan(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() > FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FColor>::Value && !TIsArithmetic<FColor>::Value, bool>::Type
IsValueGreaterThan(const FColor& InLhs, const FColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() > FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FVector>::Value && !TIsArithmetic<FVector>::Value, bool>::Type
IsValueGreaterThan(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y && InLhs.Z > InRhs.Z && InLhs.Z > InRhs.Z;
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<THasOperatorGreaterThan<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, bool>::Type
IsValueGreaterThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueLessThan not implemented");
}

template<>
FORCEINLINE typename TEnableIf<THasOperatorGreaterThan<FVector2D>::Value && !TIsArithmetic<FVector2D>::Value, bool>::Type
IsValueGreaterThan(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs > InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FVector4>::Value && !TIsArithmetic<FVector4>::Value, bool>::Type
IsValueGreaterThan(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y && InLhs.Z > InRhs.Z && InLhs.Z > InRhs.Z && InLhs.W > InRhs.W;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FPackedNormal>::Value && !TIsArithmetic<FPackedNormal>::Value, bool>::Type
IsValueGreaterThan(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FPackedRGB10A2N>::Value && !TIsArithmetic<FPackedRGB10A2N>::Value, bool>::Type
IsValueGreaterThan(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorGreaterThan<FPackedRGBA16N>::Value && !TIsArithmetic<FPackedRGBA16N>::Value, bool>::Type
IsValueGreaterThan(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsArithmetic<Ue4Type>::Value, Ue4Type>::Type
AddValues(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "AddValues not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector>::Value, FVector>::Type
AddValues(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs + InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector2D>::Value, FVector2D>::Type
AddValues(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs + InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector4>::Value, FVector4>::Type
AddValues(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs + InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FColor>::Value, FColor>::Type
AddValues(const FColor& InLhs, const FColor& InRhs)
{
    return FColor((uint8)FMath::Min((int32)InLhs.R + (int32)InRhs.R, 255),
                  (uint8)FMath::Min((int32)InLhs.G + (int32)InRhs.G, 255),
                  (uint8)FMath::Min((int32)InLhs.B + (int32)InRhs.B, 255),
                  (uint8)FMath::Min((int32)InLhs.A + (int32)InRhs.A, 255));
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FLinearColor>::Value, FLinearColor>::Type
AddValues(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs + InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedNormal>::Value, FPackedNormal>::Type
AddValues(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    const FPackedNormal Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedRGB10A2N>::Value, FPackedRGB10A2N>::Type
AddValues(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    const FPackedRGB10A2N Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedRGBA16N>::Value, FPackedRGBA16N>::Type
AddValues(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    const FPackedRGBA16N Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FIntPoint>::Value, FIntPoint>::Type
AddValues(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs + InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FIntVector>::Value, FIntVector>::Type
AddValues(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs + InRhs;
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsArithmetic<Ue4Type>::Value, Ue4Type>::Type
SubValues(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "SubValues not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector>::Value, FVector>::Type
SubValues(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs - InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector2D>::Value, FVector2D>::Type
SubValues(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs - InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FVector4>::Value, FVector4>::Type
SubValues(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs - InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FColor>::Value, FColor>::Type
SubValues(const FColor& InLhs, const FColor& InRhs)
{
    return FColor((uint8)FMath::Max((int32)InLhs.R - (int32)InRhs.R, 0),
                  (uint8)FMath::Max((int32)InLhs.G - (int32)InRhs.G, 0),
                  (uint8)FMath::Max((int32)InLhs.B - (int32)InRhs.B, 0),
                  (uint8)FMath::Max((int32)InLhs.A - (int32)InRhs.A, 0));
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FLinearColor>::Value, FLinearColor>::Type
SubValues(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs - InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedNormal>::Value, FPackedNormal>::Type
SubValues(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    const FPackedNormal Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedRGB10A2N>::Value, FPackedRGB10A2N>::Type
SubValues(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    const FPackedRGB10A2N Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FPackedRGBA16N>::Value, FPackedRGBA16N>::Type
SubValues(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    const FPackedRGBA16N Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FIntPoint>::Value, FIntPoint>::Type
SubValues(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs - InRhs;
}

template<>
FORCEINLINE typename TEnableIf<!TIsArithmetic<FIntVector>::Value, FIntVector>::Type
SubValues(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs - InRhs;
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsSigned<Ue4Type>::Value && !TIsFloatingPoint<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, Ue4Type>::Type
AbsValue(const Ue4Type& InValue)
{
    static_assert(false, "AbsValue not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector>::Value && !TIsFloatingPoint<FVector>::Value && !TIsArithmetic<FVector>::Value, FVector>::Type
AbsValue(const FVector& InValue)
{
    return InValue.GetAbs();
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector2D>::Value && !TIsFloatingPoint<FVector2D>::Value && !TIsArithmetic<FVector2D>::Value, FVector2D>::Type
AbsValue(const FVector2D& InValue)
{
    return InValue.GetAbs();
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector4>::Value && !TIsFloatingPoint<FVector4>::Value && !TIsArithmetic<FVector4>::Value, FVector4>::Type
AbsValue(const FVector4& InValue)
{
    return FVector4(FMath::Abs(InValue.X), FMath::Abs(InValue.Y), FMath::Abs(InValue.Z), FMath::Abs(InValue.W));
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FColor>::Value && !TIsFloatingPoint<FColor>::Value && !TIsArithmetic<FColor>::Value, FColor>::Type
AbsValue(const FColor& InValue)
{
    return InValue;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FLinearColor>::Value && !TIsFloatingPoint<FLinearColor>::Value && !TIsArithmetic<FLinearColor>::Value, FLinearColor>::Type
AbsValue(const FLinearColor& InValue)
{
    return FVector4(FMath::Abs(InValue.R), FMath::Abs(InValue.G), FMath::Abs(InValue.B), FMath::Abs(InValue.A));
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedNormal>::Value && !TIsFloatingPoint<FPackedNormal>::Value && !TIsArithmetic<FPackedNormal>::Value, FPackedNormal>::Type
AbsValue(const FPackedNormal& InValue)
{
    FPackedNormal Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedRGB10A2N>::Value && !TIsFloatingPoint<FPackedRGB10A2N>::Value && !TIsArithmetic<FPackedRGB10A2N>::Value, FPackedRGB10A2N>::Type
AbsValue(const FPackedRGB10A2N& InValue)
{
    FPackedRGB10A2N Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedRGBA16N>::Value && !TIsFloatingPoint<FPackedRGBA16N>::Value && !TIsArithmetic<FPackedRGBA16N>::Value, FPackedRGBA16N>::Type
AbsValue(const FPackedRGBA16N& InValue)
{
    FPackedRGBA16N Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FIntPoint>::Value && !TIsFloatingPoint<FIntPoint>::Value && !TIsArithmetic<FIntPoint>::Value, FIntPoint>::Type
AbsValue(const FIntPoint& InValue)
{
    return FIntPoint(FMath::Abs(InValue.X), FMath::Abs(InValue.Y));
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FIntVector>::Value && !TIsFloatingPoint<FIntVector>::Value && !TIsArithmetic<FIntVector>::Value, FIntVector>::Type
AbsValue(const FIntVector& InValue)
{
    return FIntVector(FMath::Abs(InValue.X), FMath::Abs(InValue.Y), FMath::Abs(InValue.Z));
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<Ue4Type>::Value && !TIsFloatingPoint<Ue4Type>::Value, Ue4Type>::Type
AddFloat(const Ue4Type& InValue, const float& InFloat)
{
    static_assert(false, "AddFloat not implemented");
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<THasOperatorPlusFloat<Ue4Type>::Value, Ue4Type>::Type
AddFloat(const Ue4Type& InValue, const float& InFloat)
{
    return InValue + InFloat;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FVector4>::Value && !TIsFloatingPoint<FVector4>::Value, FVector4>::Type
AddFloat(const FVector4& InValue, const float& InFloat)
{
    return FVector4(InValue.X + InFloat, InValue.Y + InFloat, InValue.Z + InFloat, InValue.W + InFloat);
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FColor>::Value && !TIsFloatingPoint<FColor>::Value, FColor>::Type
AddFloat(const FColor& InValue, const float& InFloat)
{
    return InValue;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FLinearColor>::Value && !TIsFloatingPoint<FLinearColor>::Value, FLinearColor>::Type
AddFloat(const FLinearColor& InValue, const float& InFloat)
{
    return FLinearColor(InValue.R + InFloat, InValue.G + InFloat, InValue.B + InFloat, InValue.A + InFloat);
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FPackedNormal>::Value && !TIsFloatingPoint<FPackedNormal>::Value, FPackedNormal>::Type
AddFloat(const FPackedNormal& InValue, const float& InFloat)
{
    const FPackedNormal Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FPackedRGB10A2N>::Value && !TIsFloatingPoint<FPackedRGB10A2N>::Value, FPackedRGB10A2N>::Type
AddFloat(const FPackedRGB10A2N& InValue, const float& InFloat)
{
    const FPackedRGB10A2N Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FPackedRGBA16N>::Value && !TIsFloatingPoint<FPackedRGBA16N>::Value, FPackedRGBA16N>::Type
AddFloat(const FPackedRGBA16N& InValue, const float& InFloat)
{
    const FPackedRGBA16N Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FIntPoint>::Value && !TIsFloatingPoint<FIntPoint>::Value, FIntPoint>::Type
AddFloat(const FIntPoint& InValue, const float& InFloat)
{
    return InValue;
}

template<>
FORCEINLINE typename TEnableIf<!THasOperatorPlusFloat<FIntVector>::Value && !TIsFloatingPoint<FIntVector>::Value, FIntVector>::Type
AddFloat(const FIntVector& InValue, const float& InFloat)
{
    return InValue;
}

template<typename Ue4Type>
FORCEINLINE typename TEnableIf<!TIsSigned<Ue4Type>::Value && !TIsFloatingPoint<Ue4Type>::Value && !TIsArithmetic<Ue4Type>::Value, Ue4Type>::Type
NegValue(const Ue4Type& InValue)
{
    static_assert(false, "NegValue not implemented");
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector>::Value && !TIsFloatingPoint<FVector>::Value && !TIsArithmetic<FVector>::Value, FVector>::Type
NegValue(const FVector& InValue)
{
    return -InValue;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector2D>::Value && !TIsFloatingPoint<FVector2D>::Value && !TIsArithmetic<FVector2D>::Value, FVector2D>::Type
NegValue(const FVector2D& InValue)
{
    return FVector2D(-InValue.X, -InValue.Y);
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FVector4>::Value && !TIsFloatingPoint<FVector4>::Value && !TIsArithmetic<FVector4>::Value, FVector4>::Type
NegValue(const FVector4& InValue)
{
    return FVector4(-InValue.X, -InValue.Y, -InValue.Z, -InValue.W);
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FColor>::Value && !TIsFloatingPoint<FColor>::Value && !TIsArithmetic<FColor>::Value, FColor>::Type
NegValue(const FColor& InValue)
{
    return InValue;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FLinearColor>::Value && !TIsFloatingPoint<FLinearColor>::Value && !TIsArithmetic<FLinearColor>::Value, FLinearColor>::Type
NegValue(const FLinearColor& InValue)
{
    return FLinearColor(-InValue.R, -InValue.G, -InValue.B, -InValue.A);
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedNormal>::Value && !TIsFloatingPoint<FPackedNormal>::Value && !TIsArithmetic<FPackedNormal>::Value, FPackedNormal>::Type
NegValue(const FPackedNormal& InValue)
{
    const FPackedNormal Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedRGB10A2N>::Value && !TIsFloatingPoint<FPackedRGB10A2N>::Value && !TIsArithmetic<FPackedRGB10A2N>::Value, FPackedRGB10A2N>::Type
NegValue(const FPackedRGB10A2N& InValue)
{
    const FPackedRGB10A2N Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FPackedRGBA16N>::Value && !TIsFloatingPoint<FPackedRGBA16N>::Value && !TIsArithmetic<FPackedRGBA16N>::Value, FPackedRGBA16N>::Type
NegValue(const FPackedRGBA16N& InValue)
{
    const FPackedRGBA16N Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FIntPoint>::Value && !TIsFloatingPoint<FIntPoint>::Value && !TIsArithmetic<FIntPoint>::Value, FIntPoint>::Type
NegValue(const FIntPoint& InValue)
{
    return FIntPoint(-InValue.X, -InValue.Y);
}

template<>
FORCEINLINE typename TEnableIf<!TIsSigned<FIntVector>::Value && !TIsFloatingPoint<FIntVector>::Value && !TIsArithmetic<FIntVector>::Value, FIntVector>::Type
NegValue(const FIntVector& InValue)
{
    return FIntVector(-InValue.X, -InValue.Y, -InValue.Z);
}

template<typename Ue4Type>
struct TVoxelDatabaseVoxelType
{
    typedef typename Ue4Type ValueType;
    typedef typename TVoxelDatabaseVoxelType<Ue4Type> Type;
    static_assert(!TIsPointer<ValueType>::Value, "Voxel types cannot be a pointer type");

    TVoxelDatabaseVoxelType()
        : Value(ZeroValue)
    {}

    TVoxelDatabaseVoxelType(ValueType&& InValue)
        : Value(MoveTemp(InValue))
    {}

    TVoxelDatabaseVoxelType(const ValueType& InValue)
        : Value(InValue)
    {}

    ValueType Value;
    const static ValueType ZeroValue;

    FORCEINLINE Type& operator=(const ValueType& InRhs)
    {
        Value = InRhs;
        return *this;
    }

    FORCEINLINE friend OutputStreamType& operator<<(OutputStreamType& os, const Type& InRhs)
    {
        WriteValue<ValueType>(os, InRhs.Value);
        return os;
    }

    FORCEINLINE friend bool operator==(const Type& InLhs, const Type& InRhs)
    {
        return AreValuesEqual<ValueType>(InLhs.Value, InRhs.Value);
    }

    FORCEINLINE friend Type Abs(const Type& InType)
    {
        return AbsValue<ValueType>(InType.Value);
    }

    FORCEINLINE friend Type operator+(const Type& InLhs, const float &InRhs)
    {
        return AddFloat<ValueType>(InLhs.Value, InRhs);
    }

    FORCEINLINE friend Type operator+(const Type& InLhs, const Type& InRhs)
    {
        return AddValues<ValueType>(InLhs.Value, InRhs.Value);
    }

    FORCEINLINE friend Type operator-(const Type& InLhs, const Type& InRhs)
    {
        return SubValues<ValueType>(InLhs.Value, InRhs.Value);
    }

    FORCEINLINE friend Type operator-(const Type& InLhs)
    {
        return NegValue<ValueType>(InLhs.Value);
    }

    FORCEINLINE friend bool operator<(const Type& InLhs, const Type &InRhs)
    {
        return IsValueLessThan<ValueType>(InLhs.Value, InRhs.Value);
    }

    FORCEINLINE friend bool operator>(const Type& InLhs, const Type &InRhs)
    {
        return IsValueGreaterThan<ValueType>(InLhs.Value, InRhs.Value);
    }
};

template<typename Ue4Type>
struct TVoxelDatabaseMetadataType
{
    typedef typename Ue4Type ValueType;
    typedef typename TVoxelDatabaseMetadataType<Ue4Type> Type;
    static_assert(!TIsPointer<ValueType>::Value, "Metadata types cannot be a pointer type");

    TVoxelDatabaseMetadataType()
        : Value(ZeroValue)
    {}

    TVoxelDatabaseMetadataType(ValueType&& InValue)
        : Value(MoveTemp(InValue))
    {}

    TVoxelDatabaseMetadataType(const ValueType& InValue)
        : Value(InValue)
    {}

    ValueType Value;
    const static ValueType ZeroValue;

    FORCEINLINE Type& operator=(const ValueType& InRhs)
    {
        Value = InRhs;
        return *this;
    }

    FORCEINLINE friend OutputStreamType& operator<<(OutputStreamType& os, const Type& InRhs)
    {
        WriteValue<ValueType>(os, InRhs.Value);
        return os;
    }

    FORCEINLINE friend bool operator==(const Type& InLhs, const Type& InRhs)
    {
        return AreValuesEqual<ValueType>(InLhs.Value, InRhs.Value);
    }
};

typedef TVoxelDatabaseVoxelType<FVector> FVoxelDatabaseVector;
typedef TVoxelDatabaseVoxelType<FVector4> FVoxelDatabaseVector4;
typedef TVoxelDatabaseVoxelType<FVector2D> FVoxelDatabaseVector2D;
typedef TVoxelDatabaseVoxelType<FColor> FVoxelDatabaseColor;
typedef TVoxelDatabaseVoxelType<FLinearColor> FVoxelDatabaseLinearColor;
typedef TVoxelDatabaseVoxelType<FPackedNormal> FVoxelDatabasePackedNormal;
typedef TVoxelDatabaseVoxelType<FPackedRGB10A2N> FVoxelDatabasePackedRGB10A2N;
typedef TVoxelDatabaseVoxelType<FPackedRGBA16N> FVoxelDatabasePackedRGBA16N;
typedef TVoxelDatabaseVoxelType<FIntPoint> FVoxelDatabaseIntPoint;
typedef TVoxelDatabaseVoxelType<FIntVector> FVoxelDatabaseIntVector;
typedef TVoxelDatabaseMetadataType<FFloatRange> FVoxelDatabaseFloatRange;
typedef TVoxelDatabaseMetadataType<FInt32Range> FVoxelDatabaseInt32Range;
typedef TVoxelDatabaseMetadataType<FFloatInterval> FVoxelDatabaseFloatInterval;
typedef TVoxelDatabaseMetadataType<FInt32Interval> FVoxelDatabaseInt32Interval;
typedef TVoxelDatabaseMetadataType<FFloatRangeBound> FVoxelDatabaseFloatRangeBound;
typedef TVoxelDatabaseMetadataType<FInt32RangeBound> FVoxelDatabaseInt32RangeBound;
typedef TVoxelDatabaseMetadataType<FTwoVectors> FVoxelDatabaseTwoVectors;
typedef TVoxelDatabaseMetadataType<FPlane> FVoxelDatabasePlane;
typedef TVoxelDatabaseMetadataType<FRotator> FVoxelDatabaseRotator;
typedef TVoxelDatabaseMetadataType<FQuat> FVoxelDatabaseQuat;
typedef TVoxelDatabaseMetadataType<FBox> FVoxelDatabaseBox;
typedef TVoxelDatabaseMetadataType<FBox2D> FVoxelDatabaseBox2D;
typedef TVoxelDatabaseMetadataType<FBoxSphereBounds> FVoxelDatabaseBoxSphereBounds;
typedef TVoxelDatabaseMetadataType<FOrientedBox> FVoxelDatabaseOrientedBox;
typedef TVoxelDatabaseMetadataType<FMatrix> FVoxelDatabaseMatrix;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointFloat> FVoxelDatabaseInterpCurvePointFloat;
typedef TVoxelDatabaseMetadataType<FInterpCurveFloat> FVoxelDatabaseInterpCurveFloat;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointVector2D> FVoxelDatabaseInterpCurvePointVector2D;
typedef TVoxelDatabaseMetadataType<FInterpCurveVector2D> FVoxelDatabaseInterpCurveVector2D;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointVector> FVoxelDatabaseInterpCurvePointVector;
typedef TVoxelDatabaseMetadataType<FInterpCurveVector> FVoxelDatabaseInterpCurveVector;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointQuat> FVoxelDatabaseInterpCurvePointQuat;
typedef TVoxelDatabaseMetadataType<FInterpCurveQuat> FVoxelDatabaseInterpCurveQuat;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointTwoVectors> FVoxelDatabaseInterpCurvePointTwoVectors;
typedef TVoxelDatabaseMetadataType<FInterpCurveTwoVectors> FVoxelDatabaseInterpCurveTwoVectors;
typedef TVoxelDatabaseMetadataType<FInterpCurvePointLinearColor> FVoxelDatabaseInterpCurvePointLinearColor;
typedef TVoxelDatabaseMetadataType<FInterpCurveLinearColor> FVoxelDatabaseInterpCurveLinearColor;
typedef TVoxelDatabaseMetadataType<FTransform> FVoxelDatabaseTransform;
typedef TVoxelDatabaseMetadataType<FRandomStream> FVoxelDatabaseRandomStream;
typedef TVoxelDatabaseMetadataType<FDateTime> FVoxelDatabaseDateTime;
typedef TVoxelDatabaseMetadataType<FTimespan> FVoxelDatabaseTimespan;
typedef TVoxelDatabaseMetadataType<FStringAssetReference> FVoxelDatabaseStringAssetReference;
typedef TVoxelDatabaseMetadataType<FStringClassReference> FVoxelDatabaseStringClassReference;
typedef TVoxelDatabaseMetadataType<FString> FVoxelDatabaseString;
typedef TVoxelDatabaseMetadataType<FName> FVoxelDatabaseName;
typedef TVoxelDatabaseMetadataType<FText> FVoxelDatabaseText;
typedef TVoxelDatabaseMetadataType<FAffineMap> FVoxelDatabaseAffineMap;
typedef TVoxelDatabaseMetadataType<FUnitaryMap> FVoxelDatabaseUnitaryMap;
typedef TVoxelDatabaseMetadataType<FScaleMap> FVoxelDatabaseScaleMap;
typedef TVoxelDatabaseMetadataType<FUniformScaleMap> FVoxelDatabaseUniformScaleMap;
typedef TVoxelDatabaseMetadataType<FTranslationMap> FVoxelDatabaseTranslationMap;
typedef TVoxelDatabaseMetadataType<FScaleTranslationMap> FVoxelDatabaseScaleTranslationMap;
typedef TVoxelDatabaseMetadataType<FUniformScaleTranslationMap> FVoxelDatabaseUniformScaleTranslationMap;
typedef TVoxelDatabaseMetadataType<FNonlinearFrustumMap> FVoxelDatabaseNonlinearFrustumMap;

template<> FORCEINLINE FVoxelDatabaseVector2D openvdb::zeroVal<FVoxelDatabaseVector2D>()
{
    return FVoxelDatabaseVector2D::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseVector openvdb::zeroVal<FVoxelDatabaseVector>()
{
    return FVoxelDatabaseVector::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseVector4 openvdb::zeroVal<FVoxelDatabaseVector4>()
{
    return FVoxelDatabaseVector4::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseColor openvdb::zeroVal<FVoxelDatabaseColor>()
{
    return FVoxelDatabaseColor::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseLinearColor openvdb::zeroVal<FVoxelDatabaseLinearColor>()
{
    return FVoxelDatabaseLinearColor::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabasePackedNormal openvdb::zeroVal<FVoxelDatabasePackedNormal>()
{
    return FVoxelDatabasePackedNormal::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabasePackedRGB10A2N openvdb::zeroVal<FVoxelDatabasePackedRGB10A2N>()
{
    return FVoxelDatabasePackedRGB10A2N::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabasePackedRGBA16N openvdb::zeroVal<FVoxelDatabasePackedRGBA16N>()
{
    return FVoxelDatabasePackedRGBA16N::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseIntPoint openvdb::zeroVal<FVoxelDatabaseIntPoint>()
{
    return FVoxelDatabaseIntPoint::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseIntVector openvdb::zeroVal<FVoxelDatabaseIntVector>()
{
    return FVoxelDatabaseIntVector::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseFloatRange openvdb::zeroVal<FVoxelDatabaseFloatRange>()
{
    return FVoxelDatabaseFloatRange::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInt32Range openvdb::zeroVal<FVoxelDatabaseInt32Range>()
{
    return FVoxelDatabaseInt32Range::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseFloatInterval openvdb::zeroVal<FVoxelDatabaseFloatInterval>()
{
    return FVoxelDatabaseFloatInterval::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInt32Interval openvdb::zeroVal<FVoxelDatabaseInt32Interval>()
{
    return FVoxelDatabaseInt32Interval::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseFloatRangeBound openvdb::zeroVal<FVoxelDatabaseFloatRangeBound>()
{
    return FVoxelDatabaseFloatRangeBound::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInt32RangeBound openvdb::zeroVal<FVoxelDatabaseInt32RangeBound>()
{
    return FVoxelDatabaseInt32RangeBound::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseTwoVectors openvdb::zeroVal<FVoxelDatabaseTwoVectors>()
{
    return FVoxelDatabaseTwoVectors::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabasePlane openvdb::zeroVal<FVoxelDatabasePlane>()
{
    return FVoxelDatabasePlane::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseRotator openvdb::zeroVal<FVoxelDatabaseRotator>()
{
    return FVoxelDatabaseRotator::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseQuat openvdb::zeroVal<FVoxelDatabaseQuat>()
{
    return FVoxelDatabaseQuat::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseBox openvdb::zeroVal<FVoxelDatabaseBox>()
{
    return FVoxelDatabaseBox::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseBox2D openvdb::zeroVal<FVoxelDatabaseBox2D>()
{
    return FVoxelDatabaseBox2D::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseBoxSphereBounds openvdb::zeroVal<FVoxelDatabaseBoxSphereBounds>()
{
    return FVoxelDatabaseBoxSphereBounds::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseOrientedBox openvdb::zeroVal<FVoxelDatabaseOrientedBox>()
{
    return FVoxelDatabaseOrientedBox::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseMatrix openvdb::zeroVal<FVoxelDatabaseMatrix>()
{
    return FVoxelDatabaseMatrix::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointFloat openvdb::zeroVal<FVoxelDatabaseInterpCurvePointFloat>()
{
    return FVoxelDatabaseInterpCurvePointFloat::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveFloat openvdb::zeroVal<FVoxelDatabaseInterpCurveFloat>()
{
    return FVoxelDatabaseInterpCurveFloat::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointVector2D openvdb::zeroVal<FVoxelDatabaseInterpCurvePointVector2D>()
{
    return FVoxelDatabaseInterpCurvePointVector2D::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveVector2D openvdb::zeroVal<FVoxelDatabaseInterpCurveVector2D>()
{
    return FVoxelDatabaseInterpCurveVector2D::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointVector openvdb::zeroVal<FVoxelDatabaseInterpCurvePointVector>()
{
    return FVoxelDatabaseInterpCurvePointVector::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveVector openvdb::zeroVal<FVoxelDatabaseInterpCurveVector>()
{
    return FVoxelDatabaseInterpCurveVector::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointQuat openvdb::zeroVal<FVoxelDatabaseInterpCurvePointQuat>()
{
    return FVoxelDatabaseInterpCurvePointQuat::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveQuat openvdb::zeroVal<FVoxelDatabaseInterpCurveQuat>()
{
    return FVoxelDatabaseInterpCurveQuat::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointTwoVectors openvdb::zeroVal<FVoxelDatabaseInterpCurvePointTwoVectors>()
{
    return FVoxelDatabaseInterpCurvePointTwoVectors::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveTwoVectors openvdb::zeroVal<FVoxelDatabaseInterpCurveTwoVectors>()
{
    return FVoxelDatabaseInterpCurveTwoVectors::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurvePointLinearColor openvdb::zeroVal<FVoxelDatabaseInterpCurvePointLinearColor>()
{
    return FVoxelDatabaseInterpCurvePointLinearColor::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseInterpCurveLinearColor openvdb::zeroVal<FVoxelDatabaseInterpCurveLinearColor>()
{
    return FVoxelDatabaseInterpCurveLinearColor::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseTransform openvdb::zeroVal<FVoxelDatabaseTransform>()
{
    return FVoxelDatabaseTransform::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseRandomStream openvdb::zeroVal<FVoxelDatabaseRandomStream>()
{
    return FVoxelDatabaseRandomStream::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseDateTime openvdb::zeroVal<FVoxelDatabaseDateTime>()
{
    return FVoxelDatabaseDateTime::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseTimespan openvdb::zeroVal<FVoxelDatabaseTimespan>()
{
    return FVoxelDatabaseTimespan::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseStringAssetReference openvdb::zeroVal<FVoxelDatabaseStringAssetReference>()
{
    return FVoxelDatabaseStringAssetReference::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseStringClassReference openvdb::zeroVal<FVoxelDatabaseStringClassReference>()
{
    return FVoxelDatabaseStringClassReference::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseString openvdb::zeroVal<FVoxelDatabaseString>()
{
    return FVoxelDatabaseString::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseName openvdb::zeroVal<FVoxelDatabaseName>()
{
    return FVoxelDatabaseName::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseText openvdb::zeroVal<FVoxelDatabaseText>()
{
    return FVoxelDatabaseText::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseAffineMap openvdb::zeroVal<FVoxelDatabaseAffineMap>()
{
    return FVoxelDatabaseAffineMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseUnitaryMap openvdb::zeroVal<FVoxelDatabaseUnitaryMap>()
{
    return FVoxelDatabaseUnitaryMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseScaleMap openvdb::zeroVal<FVoxelDatabaseScaleMap>()
{
    return FVoxelDatabaseScaleMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseUniformScaleMap openvdb::zeroVal<FVoxelDatabaseUniformScaleMap>()
{
    return FVoxelDatabaseUniformScaleMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseTranslationMap openvdb::zeroVal<FVoxelDatabaseTranslationMap>()
{
    return FVoxelDatabaseTranslationMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseScaleTranslationMap openvdb::zeroVal<FVoxelDatabaseScaleTranslationMap>()
{
    return FVoxelDatabaseScaleTranslationMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseUniformScaleTranslationMap openvdb::zeroVal<FVoxelDatabaseUniformScaleTranslationMap>()
{
    return FVoxelDatabaseUniformScaleTranslationMap::ZeroValue;
}

template<> FORCEINLINE FVoxelDatabaseNonlinearFrustumMap openvdb::zeroVal<FVoxelDatabaseNonlinearFrustumMap>()
{
    return FVoxelDatabaseNonlinearFrustumMap::ZeroValue;
}

template<> struct openvdb::math::Tolerance<FVoxelDatabaseVector>
{
    FORCEINLINE static FVoxelDatabaseVector value() { return FVector(openvdb::math::Tolerance<float>::value()); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabaseVector4>
{
    FORCEINLINE static FVoxelDatabaseVector4 value() { return FVector4(openvdb::math::Tolerance<float>::value()); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabaseVector2D>
{
    FORCEINLINE static FVoxelDatabaseVector2D value() { return FVector2D(openvdb::math::Tolerance<float>::value(), openvdb::math::Tolerance<float>::value()); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabaseLinearColor>
{
    FORCEINLINE static FVoxelDatabaseLinearColor value() { return FLinearColor(FVector4(openvdb::math::Tolerance<float>::value())); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedNormal>
{
    FORCEINLINE static FVoxelDatabasePackedNormal value() { return FPackedNormal(FVector4(openvdb::math::Tolerance<float>::value())); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedRGB10A2N>
{
    FORCEINLINE static FVoxelDatabasePackedRGB10A2N value() { return FVoxelDatabasePackedRGB10A2N(FVector4(openvdb::math::Tolerance<float>::value())); }
};

template<> struct openvdb::math::Tolerance<FVoxelDatabasePackedRGBA16N>
{
    FORCEINLINE static FVoxelDatabasePackedRGBA16N value() { return FVoxelDatabasePackedRGBA16N(FVector4(openvdb::math::Tolerance<float>::value())); }
};

template<> struct openvdb::math::Delta<FVoxelDatabaseVector>
{
    FORCEINLINE static FVoxelDatabaseVector value() { return FVector(openvdb::math::Delta<float>::value()); }
};

template<> struct openvdb::math::Delta<FVoxelDatabaseVector4>
{
    FORCEINLINE static FVoxelDatabaseVector4 value() { return FVector4(openvdb::math::Delta<float>::value()); }
};

template<> struct openvdb::math::Delta<FVoxelDatabaseVector2D>
{
    FORCEINLINE static FVoxelDatabaseVector2D value() { return FVector2D(openvdb::math::Delta<float>::value(), openvdb::math::Delta<float>::value()); }
};

template<> struct openvdb::math::Delta<FVoxelDatabaseLinearColor>
{
    FORCEINLINE static FVoxelDatabaseLinearColor value() { return FLinearColor(FVector4(openvdb::math::Delta<float>::value())); }
};

template<> struct openvdb::math::Delta<FVoxelDatabasePackedNormal>
{
    FORCEINLINE static FVoxelDatabasePackedNormal value() { return FPackedNormal(FVector4(openvdb::math::Delta<float>::value())); }
};

template<> struct openvdb::math::Delta<FVoxelDatabasePackedRGB10A2N>
{
    FORCEINLINE static FVoxelDatabasePackedRGB10A2N value() { return FVoxelDatabasePackedRGB10A2N(FVector4(openvdb::math::Delta<float>::value())); }
};

template<> struct openvdb::math::Delta<FVoxelDatabasePackedRGBA16N>
{
    FORCEINLINE static FVoxelDatabasePackedRGBA16N value() { return FVoxelDatabasePackedRGBA16N(FVector4(openvdb::math::Delta<float>::value())); }
};

#pragma warning(pop)
