#pragma once
#include "Platform.h"
#include "EngineMinimal.h"
#include "GridCoordinateTransforms.h"

#include <ostream>
typedef std::ostream OutputStreamType;

namespace VoxelMateGridTypes {

template<typename Ue4Type>
VOXELMATEINLINE void WriteValue(OutputStreamType& os, const Ue4Type& InValue)
{
    static_assert(false, "WriteValue not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE bool AreValuesEqual(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "AreValuesEqual not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE bool IsValueLessThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueLessThan not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE bool IsValueGreaterThan(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "IsValueGreaterThan not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE Ue4Type AddValues(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "AddValues not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE Ue4Type SubValues(const Ue4Type& InLhs, const Ue4Type& InRhs)
{
    static_assert(false, "SubValues not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE Ue4Type AbsValue(const Ue4Type& InValue)
{
    static_assert(false, "AbsValue not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE Ue4Type AddFloat(const Ue4Type& InValue, const float& InFloat)
{
    static_assert(false, "AddFloat not implemented");
}

template<typename Ue4Type>
VOXELMATEINLINE Ue4Type NegValue(const Ue4Type& InValue)
{
    static_assert(false, "NegValue not implemented");
}

template<>
VOXELMATEINLINE void WriteValue<bool>(OutputStreamType& os, const bool& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(bool));
}

template<>
VOXELMATEINLINE void WriteValue<double>(OutputStreamType& os, const double& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(double));
}

template<>
VOXELMATEINLINE void WriteValue<float>(OutputStreamType& os, const float& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(float));
}

template<>
VOXELMATEINLINE void WriteValue<int8>(OutputStreamType& os, const int8& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(int8));
}

template<>
VOXELMATEINLINE void WriteValue<int16>(OutputStreamType& os, const int16& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(int16));
}

template<>
VOXELMATEINLINE void WriteValue<int32>(OutputStreamType& os, const int32& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(int32));
}

template<>
VOXELMATEINLINE void WriteValue<int64>(OutputStreamType& os, const int64& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(int64));
}

template<>
VOXELMATEINLINE void WriteValue<uint8>(OutputStreamType& os, const uint8& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(uint8));
}

template<>
VOXELMATEINLINE void WriteValue<uint16>(OutputStreamType& os, const uint16& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(uint16));
}

template<>
VOXELMATEINLINE void WriteValue<uint32>(OutputStreamType& os, const uint32& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(uint32));
}

template<>
VOXELMATEINLINE void WriteValue<uint64>(OutputStreamType& os, const uint64& InValue)
{
    const char* Bytes = reinterpret_cast<const char*>(&InValue);
    os.write(Bytes, sizeof(uint64));
}

template<>
VOXELMATEINLINE void WriteValue<FVector>(OutputStreamType& os, const FVector& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
}

template<>
VOXELMATEINLINE void WriteValue<FVector4>(OutputStreamType& os, const FVector4& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
VOXELMATEINLINE void WriteValue<FVector2D>(OutputStreamType& os, const FVector2D& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
}

template<>
VOXELMATEINLINE void WriteValue<FColor>(OutputStreamType& os, const FColor& InValue)
{
    WriteValue<uint32>(os, InValue.DWColor());
}

template<>
VOXELMATEINLINE void WriteValue<FLinearColor>(OutputStreamType& os, const FLinearColor& InValue)
{
    WriteValue<float>(os, InValue.R);
    WriteValue<float>(os, InValue.G);
    WriteValue<float>(os, InValue.B);
    WriteValue<float>(os, InValue.A);
}

template<>
VOXELMATEINLINE void WriteValue<FPackedNormal>(OutputStreamType& os, const FPackedNormal& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
VOXELMATEINLINE void WriteValue<FPackedRGB10A2N>(OutputStreamType& os, const FPackedRGB10A2N& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
VOXELMATEINLINE void WriteValue<FPackedRGBA16N>(OutputStreamType& os, const FPackedRGBA16N& InValue)
{
    WriteValue<FVector4>(os, FVector4(InValue));
}

template<>
VOXELMATEINLINE void WriteValue<FIntPoint>(OutputStreamType& os, const FIntPoint& InValue)
{
    WriteValue<int32>(os, InValue.X);
    WriteValue<int32>(os, InValue.Y);
}

template<>
VOXELMATEINLINE void WriteValue<FIntVector>(OutputStreamType& os, const FIntVector& InValue)
{
    WriteValue<int32>(os, InValue.X);
    WriteValue<int32>(os, InValue.Y);
    WriteValue<int32>(os, InValue.Z);
}

template<>
VOXELMATEINLINE void WriteValue<FFloatRangeBound>(OutputStreamType& os, const FFloatRangeBound& InValue)
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
VOXELMATEINLINE void WriteValue<FFloatRange>(OutputStreamType& os, const FFloatRange& InValue)
{
    WriteValue<FFloatRangeBound>(os, InValue.GetLowerBoundValue());
    WriteValue<FFloatRangeBound>(os, InValue.GetUpperBoundValue());
}

template<>
VOXELMATEINLINE void WriteValue<FInt32RangeBound>(OutputStreamType& os, const FInt32RangeBound& InValue)
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
VOXELMATEINLINE void WriteValue<FInt32Range>(OutputStreamType& os, const FInt32Range& InValue)
{
    WriteValue<FInt32RangeBound>(os, InValue.GetLowerBoundValue());
    WriteValue<FInt32RangeBound>(os, InValue.GetUpperBoundValue());
}

template<>
VOXELMATEINLINE void WriteValue<FFloatInterval>(OutputStreamType& os, const FFloatInterval& InValue)
{
    WriteValue<float>(os, InValue.Min);
    WriteValue<float>(os, InValue.Max);
}

template<>
VOXELMATEINLINE void WriteValue<FInt32Interval>(OutputStreamType& os, const FInt32Interval& InValue)
{
    WriteValue<int32>(os, InValue.Min);
    WriteValue<int32>(os, InValue.Max);
}

template<>
VOXELMATEINLINE void WriteValue<FTwoVectors>(OutputStreamType& os, const FTwoVectors& InValue)
{
    WriteValue<FVector>(os, InValue.v1);
    WriteValue<FVector>(os, InValue.v2);
}

template<>
VOXELMATEINLINE void WriteValue<FPlane>(OutputStreamType& os, const FPlane& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
VOXELMATEINLINE void WriteValue<FRotator>(OutputStreamType& os, const FRotator& InValue)
{
    WriteValue<float>(os, InValue.Pitch);
    WriteValue<float>(os, InValue.Roll);
    WriteValue<float>(os, InValue.Yaw);
}

template<>
VOXELMATEINLINE void WriteValue<FQuat>(OutputStreamType& os, const FQuat& InValue)
{
    WriteValue<float>(os, InValue.X);
    WriteValue<float>(os, InValue.Y);
    WriteValue<float>(os, InValue.Z);
    WriteValue<float>(os, InValue.W);
}

template<>
VOXELMATEINLINE void WriteValue<FBox>(OutputStreamType& os, const FBox& InValue)
{
    WriteValue<FVector>(os, InValue.Min);
    WriteValue<FVector>(os, InValue.Max);
}

template<>
VOXELMATEINLINE void WriteValue<FBox2D>(OutputStreamType& os, const FBox2D& InValue)
{
    WriteValue<FVector2D>(os, InValue.Min);
    WriteValue<FVector2D>(os, InValue.Max);
}

template<>
VOXELMATEINLINE void WriteValue<FBoxSphereBounds>(OutputStreamType& os, const FBoxSphereBounds& InValue)
{
    WriteValue<FVector>(os, InValue.Origin);
    WriteValue<FVector>(os, InValue.BoxExtent);
    WriteValue<float>(os, InValue.SphereRadius);
}

template<>
VOXELMATEINLINE void WriteValue<FOrientedBox>(OutputStreamType& os, const FOrientedBox& InValue)
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
VOXELMATEINLINE void WriteValue<FMatrix>(OutputStreamType& os, const FMatrix& InValue)
{
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(0));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(1));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(2));
    WriteValue<FVector>(os, InValue.FMatrix::GetColumn(3));
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointFloat>(OutputStreamType& os, const FInterpCurvePointFloat& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<float>(os, InValue.OutVal);
    WriteValue<float>(os, InValue.ArriveTangent);
    WriteValue<float>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveFloat>(OutputStreamType& os, const FInterpCurveFloat& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointFloat>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointVector2D>(OutputStreamType& os, const FInterpCurvePointVector2D& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FVector2D>(os, InValue.OutVal);
    WriteValue<FVector2D>(os, InValue.ArriveTangent);
    WriteValue<FVector2D>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveVector2D>(OutputStreamType& os, const FInterpCurveVector2D& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointVector2D>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointVector>(OutputStreamType& os, const FInterpCurvePointVector& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FVector>(os, InValue.OutVal);
    WriteValue<FVector>(os, InValue.ArriveTangent);
    WriteValue<FVector>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveVector>(OutputStreamType& os, const FInterpCurveVector& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointVector>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointQuat>(OutputStreamType& os, const FInterpCurvePointQuat& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FQuat>(os, InValue.OutVal);
    WriteValue<FQuat>(os, InValue.ArriveTangent);
    WriteValue<FQuat>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveQuat>(OutputStreamType& os, const FInterpCurveQuat& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointQuat>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointTwoVectors>(OutputStreamType& os, const FInterpCurvePointTwoVectors& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FTwoVectors>(os, InValue.OutVal);
    WriteValue<FTwoVectors>(os, InValue.ArriveTangent);
    WriteValue<FTwoVectors>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveTwoVectors>(OutputStreamType& os, const FInterpCurveTwoVectors& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointTwoVectors>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurvePointLinearColor>(OutputStreamType& os, const FInterpCurvePointLinearColor& InValue)
{
    WriteValue<float>(os, InValue.InVal);
    WriteValue<FLinearColor>(os, InValue.OutVal);
    WriteValue<FLinearColor>(os, InValue.ArriveTangent);
    WriteValue<FLinearColor>(os, InValue.LeaveTangent);
    WriteValue<uint8>(os, InValue.InterpMode);
}

template<>
VOXELMATEINLINE void WriteValue<FInterpCurveLinearColor>(OutputStreamType& os, const FInterpCurveLinearColor& InValue)
{
    for (auto i = InValue.Points.CreateConstIterator(); i; ++i)
    {
        WriteValue<FInterpCurvePointLinearColor>(os, *i);
    }
    WriteValue<bool>(os, InValue.bIsLooped);
    WriteValue<float>(os, InValue.LoopKeyOffset);
}

template<>
VOXELMATEINLINE void WriteValue<FTransform>(OutputStreamType& os, const FTransform& InValue)
{
    WriteValue<FQuat>(os, InValue.GetRotation());
    WriteValue<FVector>(os, InValue.GetTranslation());
    WriteValue<FVector>(os, InValue.GetScale3D());
}

template<>
VOXELMATEINLINE void WriteValue<FRandomStream>(OutputStreamType& os, const FRandomStream& InValue)
{
    WriteValue<int32>(os, InValue.GetCurrentSeed());
}

template<>
VOXELMATEINLINE void WriteValue(OutputStreamType& os, const FDateTime& InValue)
{
    WriteValue<int64>(os, InValue.GetTicks());
}

template<>
VOXELMATEINLINE void WriteValue<FTimespan>(OutputStreamType& os, const FTimespan& InValue)
{
    WriteValue<int64>(os, InValue.GetTicks());
}

template<>
VOXELMATEINLINE void WriteValue<FString>(OutputStreamType& os, const FString& InValue)
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
VOXELMATEINLINE void WriteValue<FStringAssetReference>(OutputStreamType& os, const FStringAssetReference& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
VOXELMATEINLINE void WriteValue<FStringClassReference>(OutputStreamType& os, const FStringClassReference& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
VOXELMATEINLINE void WriteValue<FName>(OutputStreamType& os, const FName& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
VOXELMATEINLINE void WriteValue<FText>(OutputStreamType& os, const FText& InValue)
{
    WriteValue<FString>(os, InValue.ToString());
}

template<>
VOXELMATEINLINE void WriteValue<FAffineCoordinateTransform>(OutputStreamType& os, const FAffineCoordinateTransform& InValue)
{
    WriteValue<FMatrix>(os, InValue.Matrix);
}

template<>
VOXELMATEINLINE void WriteValue<FUnitaryCoordinateTransform>(OutputStreamType& os, const FUnitaryCoordinateTransform& InValue)
{
    WriteValue<FVector>(os, InValue.Quat.GetRotationAxis());
    WriteValue<float>(os, 2.f * FMath::Acos(InValue.Quat.W));
}

template<>
VOXELMATEINLINE void WriteValue<FScaleCoordinateTransform>(OutputStreamType& os, const FScaleCoordinateTransform& InValue)
{
    WriteValue<FVector>(os, InValue.ScaleVec);
}

template<>
VOXELMATEINLINE void WriteValue<FUniformScaleCoordinateTransform>(OutputStreamType& os, const FUniformScaleCoordinateTransform& InValue)
{
    WriteValue<float>(os, InValue.ScaleValue);
}

template<>
VOXELMATEINLINE void WriteValue<FTranslationCoordinateTransform>(OutputStreamType& os, const FTranslationCoordinateTransform& InValue)
{
    WriteValue<FVector>(os, InValue.TranslationVec);
}

template<>
VOXELMATEINLINE void WriteValue<FScaleTranslationCoordinateTransform>(OutputStreamType& os, const FScaleTranslationCoordinateTransform& InValue)
{
    WriteValue<FVector>(os, InValue.ScaleVec);
    WriteValue<FVector>(os, InValue.TranslationVec);
}

template<>
VOXELMATEINLINE void WriteValue<FUniformScaleTranslationCoordinateTransform>(OutputStreamType& os, const FUniformScaleTranslationCoordinateTransform& InValue)
{
    WriteValue<float>(os, InValue.ScaleValue);
    WriteValue<FVector>(os, InValue.TranslationVec);
}

template<>
VOXELMATEINLINE void WriteValue<FNonlinearFrustumCoordinateTransform>(OutputStreamType& os, const FNonlinearFrustumCoordinateTransform& InValue)
{
    WriteValue<FBox>(os, InValue.Box);
    WriteValue<float>(os, InValue.Taper);
    WriteValue<float>(os, InValue.Depth);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<bool>(const bool& InLhs, const bool& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<double>(const double& InLhs, const double& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<float>(const float& InLhs, const float& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<int8>(const int8& InLhs, const int8& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<int16>(const int16& InLhs, const int16& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<int32>(const int32& InLhs, const int32& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<int64>(const int64& InLhs, const int64& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<uint8>(const uint8& InLhs, const uint8& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<uint16>(const uint16& InLhs, const uint16& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<uint32>(const uint32& InLhs, const uint32& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<uint64>(const uint64& InLhs, const uint64& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FVector>(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FVector4>(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FVector2D>(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FColor>(const FColor& InLhs, const FColor& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FLinearColor>(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FPackedNormal>(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FPackedRGB10A2N>(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FPackedRGBA16N>(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FIntPoint>(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FIntVector>(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FFloatRange>(const FFloatRange& InLhs, const FFloatRange& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInt32Range>(const FInt32Range& InLhs, const FInt32Range& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FFloatInterval>(const FFloatInterval& InLhs, const FFloatInterval& InRhs)
{
    return FMath::IsNearlyEqual(InLhs.Min, InRhs.Min) && FMath::IsNearlyEqual(InLhs.Max, InRhs.Max);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInt32Interval>(const FInt32Interval& InLhs, const FInt32Interval& InRhs)
{
    return InLhs.Min == InRhs.Min && InLhs.Max == InRhs.Max;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FFloatRangeBound>(const FFloatRangeBound& InLhs, const FFloatRangeBound& InRhs)
{
    return (InLhs.IsOpen() && InRhs.IsOpen()) ||
           (!InLhs.IsOpen() && !InRhs.IsOpen() && InLhs.GetValue() == InRhs.GetValue());
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInt32RangeBound>(const FInt32RangeBound& InLhs, const FInt32RangeBound& InRhs)
{
    return (InLhs.IsOpen() && InRhs.IsOpen()) ||
           (!InLhs.IsOpen() && !InRhs.IsOpen() && InLhs.GetValue() == InRhs.GetValue());
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FTwoVectors>(const FTwoVectors& InLhs, const FTwoVectors& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FPlane>(const FPlane& InLhs, const FPlane& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FRotator>(const FRotator& InLhs, const FRotator& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FQuat>(const FQuat& InLhs, const FQuat& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FBox>(const FBox& InLhs, const FBox& InRhs)
{
    return InLhs.Min.Equals(InRhs.Min) && InLhs.Max.Equals(InRhs.Max);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FBox2D>(const FBox2D& InLhs, const FBox2D& InRhs)
{
    return InLhs.Min.Equals(InRhs.Min) && InLhs.Max.Equals(InRhs.Max);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FBoxSphereBounds>(const FBoxSphereBounds& InLhs, const FBoxSphereBounds& InRhs)
{
    return InLhs.Origin.Equals(InRhs.Origin) &&
           InLhs.BoxExtent.Equals(InRhs.BoxExtent) &&
           FMath::IsNearlyEqual(InLhs.SphereRadius, InRhs.SphereRadius);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FOrientedBox>(const FOrientedBox& InLhs, const FOrientedBox& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FMatrix>(const FMatrix& InLhs, const FMatrix& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointFloat>(const FInterpCurvePointFloat& InLhs, const FInterpCurvePointFloat& InRhs)
{
    return FMath::IsNearlyEqual(InLhs.ArriveTangent, InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           FMath::IsNearlyEqual(InLhs.OutVal, InRhs.OutVal) &&
           FMath::IsNearlyEqual(InLhs.ArriveTangent, InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.LeaveTangent, InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveFloat>(const FInterpCurveFloat& InLhs, const FInterpCurveFloat& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointVector2D>(const FInterpCurvePointVector2D& InLhs, const FInterpCurvePointVector2D& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveVector2D>(const FInterpCurveVector2D& InLhs, const FInterpCurveVector2D& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointVector>(const FInterpCurvePointVector& InLhs, const FInterpCurvePointVector& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveVector>(const FInterpCurveVector& InLhs, const FInterpCurveVector& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointQuat>(const FInterpCurvePointQuat& InLhs, const FInterpCurvePointQuat& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveQuat>(const FInterpCurveQuat& InLhs, const FInterpCurveQuat& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointTwoVectors>(const FInterpCurvePointTwoVectors& InLhs, const FInterpCurvePointTwoVectors& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveTwoVectors>(const FInterpCurveTwoVectors& InLhs, const FInterpCurveTwoVectors& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FInterpCurvePointLinearColor>(const FInterpCurvePointLinearColor& InLhs, const FInterpCurvePointLinearColor& InRhs)
{
    return InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           FMath::IsNearlyEqual(InLhs.InVal, InRhs.InVal) &&
           InLhs.OutVal.Equals(InRhs.OutVal) &&
           InLhs.ArriveTangent.Equals(InRhs.ArriveTangent) &&
           InLhs.LeaveTangent.Equals(InRhs.LeaveTangent) &&
           InLhs.InterpMode == InRhs.InterpMode;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FInterpCurveLinearColor>(const FInterpCurveLinearColor& InLhs, const FInterpCurveLinearColor& InRhs)
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
VOXELMATEINLINE bool AreValuesEqual<FTransform>(const FTransform& InLhs, const FTransform& InRhs)
{
    return InLhs.Equals(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FRandomStream>(const FRandomStream& InLhs, const FRandomStream& InRhs)
{
    return InLhs.GetCurrentSeed() == InRhs.GetCurrentSeed();
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FDateTime>(const FDateTime& InLhs, const FDateTime& InRhs)
{
    return InLhs.GetTicks() == InRhs.GetTicks();
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FTimespan>(const FTimespan& InLhs, const FTimespan& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FStringAssetReference>(const FStringAssetReference& InLhs, const FStringAssetReference& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FStringClassReference>(const FStringClassReference& InLhs, const FStringClassReference& InRhs)
{
    return InLhs.ToString() == InRhs.ToString();
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FString>(const FString& InLhs, const FString& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FName>(const FName& InLhs, const FName& InRhs)
{
    return InLhs == InRhs;
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FText>(const FText& InLhs, const FText& InRhs)
{
    return InLhs.EqualTo(InRhs);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FAffineCoordinateTransform>(const FAffineCoordinateTransform& InLhs, const FAffineCoordinateTransform& InRhs)
{
    return AreValuesEqual<FMatrix>(InLhs.Matrix, InRhs.Matrix);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FUnitaryCoordinateTransform>(const FUnitaryCoordinateTransform& InLhs, const FUnitaryCoordinateTransform& InRhs)
{
    return AreValuesEqual<FQuat>(InLhs.Quat, InRhs.Quat);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FScaleCoordinateTransform>(const FScaleCoordinateTransform& InLhs, const FScaleCoordinateTransform& InRhs)
{
    return AreValuesEqual<FVector>(InLhs.ScaleVec, InRhs.ScaleVec);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FUniformScaleCoordinateTransform>(const FUniformScaleCoordinateTransform& InLhs, const FUniformScaleCoordinateTransform& InRhs)
{
    return AreValuesEqual<float>(InLhs.ScaleValue, InRhs.ScaleValue);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FTranslationCoordinateTransform>(const FTranslationCoordinateTransform& InLhs, const FTranslationCoordinateTransform& InRhs)
{
    return AreValuesEqual<FVector>(InLhs.TranslationVec, InRhs.TranslationVec);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FScaleTranslationCoordinateTransform>(const FScaleTranslationCoordinateTransform& InLhs, const FScaleTranslationCoordinateTransform& InRhs)
{
    return AreValuesEqual<FVector>(InLhs.ScaleVec, InRhs.ScaleVec) &&
           AreValuesEqual<FVector>(InLhs.TranslationVec, InRhs.TranslationVec);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FUniformScaleTranslationCoordinateTransform>(const FUniformScaleTranslationCoordinateTransform& InLhs, const FUniformScaleTranslationCoordinateTransform& InRhs)
{
    return AreValuesEqual<float>(InLhs.ScaleValue, InRhs.ScaleValue) &&
           AreValuesEqual<FVector>(InLhs.TranslationVec, InRhs.TranslationVec);
}

template<>
VOXELMATEINLINE bool AreValuesEqual<FNonlinearFrustumCoordinateTransform>(const FNonlinearFrustumCoordinateTransform& InLhs, const FNonlinearFrustumCoordinateTransform& InRhs)
{
    return AreValuesEqual<FBox>(InLhs.Box, InRhs.Box) &&
           AreValuesEqual<float>(InLhs.Taper, InRhs.Taper) &&
           AreValuesEqual<float>(InLhs.Depth, InRhs.Depth);
}

template<>
VOXELMATEINLINE bool IsValueLessThan<bool>(const bool& InLhs, const bool& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<double>(const double& InLhs, const double& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<float>(const float& InLhs, const float& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<int8>(const int8& InLhs, const int8& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<int16>(const int16& InLhs, const int16& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<int32>(const int32& InLhs, const int32& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<int64>(const int64& InLhs, const int64& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<uint8>(const uint8& InLhs, const uint8& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<uint16>(const uint16& InLhs, const uint16& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<uint32>(const uint32& InLhs, const uint32& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<uint64>(const uint64& InLhs, const uint64& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FLinearColor>(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() < FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FColor>(const FColor& InLhs, const FColor& InRhs)
{
    return InLhs.DWColor() < InRhs.DWColor();
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FVector>(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y && InLhs.Z < InRhs.Z;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FVector2D>(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs < InRhs;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FVector4>(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y && InLhs.Z < InRhs.Z && InLhs.Z < InRhs.Z;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FPackedNormal>(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FPackedRGB10A2N>(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FPackedRGBA16N>(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return IsValueLessThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FIntPoint>(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y;
}

template<>
VOXELMATEINLINE bool IsValueLessThan<FIntVector>(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs.X < InRhs.X && InLhs.Y < InRhs.Y && InLhs.Z < InRhs.Z;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<bool>(const bool& InLhs, const bool& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<double>(const double& InLhs, const double& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<float>(const float& InLhs, const float& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<int8>(const int8& InLhs, const int8& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<int16>(const int16& InLhs, const int16& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<int32>(const int32& InLhs, const int32& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<int64>(const int64& InLhs, const int64& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<uint8>(const uint8& InLhs, const uint8& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<uint16>(const uint16& InLhs, const uint16& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<uint32>(const uint32& InLhs, const uint32& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<uint64>(const uint64& InLhs, const uint64& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FLinearColor>(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return FVector4(InLhs.R, InLhs.G, InLhs.G, InLhs.A).SizeSquared3() > FVector4(InRhs.R, InRhs.G, InRhs.G, InRhs.A).SizeSquared3();
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FColor>(const FColor& InLhs, const FColor& InRhs)
{
    return InLhs.DWColor() > InRhs.DWColor();
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FVector>(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y && InLhs.Z > InRhs.Z && InLhs.Z > InRhs.Z;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FVector2D>(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs > InRhs;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FVector4>(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y && InLhs.Z > InRhs.Z && InLhs.Z > InRhs.Z && InLhs.W > InRhs.W;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FPackedNormal>(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FPackedRGB10A2N>(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FPackedRGBA16N>(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    return IsValueGreaterThan<FVector>(FVector(InLhs), FVector(InRhs));
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FIntPoint>(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y;
}

template<>
VOXELMATEINLINE bool IsValueGreaterThan<FIntVector>(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs.X > InRhs.X && InLhs.Y > InRhs.Y && InLhs.Z > InRhs.Z;
}

template<>
VOXELMATEINLINE bool AddValues<bool>(const bool& InLhs, const bool& InRhs)
{
    return InLhs;
}

template<>
VOXELMATEINLINE double AddValues<double>(const double& InLhs, const double& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE float AddValues<float>(const float& InLhs, const float& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE int8 AddValues<int8>(const int8& InLhs, const int8& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE int16 AddValues<int16>(const int16& InLhs, const int16& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE int32 AddValues<int32>(const int32& InLhs, const int32& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE int64 AddValues<int64>(const int64& InLhs, const int64& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE uint8 AddValues<uint8>(const uint8& InLhs, const uint8& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE uint16 AddValues<uint16>(const uint16& InLhs, const uint16& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE uint32 AddValues<uint32>(const uint32& InLhs, const uint32& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE uint64 AddValues<uint64>(const uint64& InLhs, const uint64& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FVector AddValues<FVector>(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FVector2D AddValues<FVector2D>(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FVector4 AddValues<FVector4>(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FColor AddValues<FColor>(const FColor& InLhs, const FColor& InRhs)
{
    return FColor((uint8)FMath::Min((int32)InLhs.R + (int32)InRhs.R, 255),
                  (uint8)FMath::Min((int32)InLhs.G + (int32)InRhs.G, 255),
                  (uint8)FMath::Min((int32)InLhs.B + (int32)InRhs.B, 255),
                  (uint8)FMath::Min((int32)InLhs.A + (int32)InRhs.A, 255));
}

template<>
VOXELMATEINLINE FLinearColor AddValues<FLinearColor>(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FPackedNormal AddValues<FPackedNormal>(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    const FPackedNormal Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGB10A2N AddValues<FPackedRGB10A2N>(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    const FPackedRGB10A2N Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGBA16N AddValues<FPackedRGBA16N>(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    const FPackedRGBA16N Normal = FVector4(InLhs) + FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FIntPoint AddValues<FIntPoint>(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE FIntVector AddValues<FIntVector>(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs + InRhs;
}

template<>
VOXELMATEINLINE bool SubValues<bool>(const bool& InLhs, const bool& InRhs)
{
    return InLhs;
}

template<>
VOXELMATEINLINE double SubValues<double>(const double& InLhs, const double& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE float SubValues<float>(const float& InLhs, const float& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE int8 SubValues<int8>(const int8& InLhs, const int8& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE int16 SubValues<int16>(const int16& InLhs, const int16& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE int32 SubValues<int32>(const int32& InLhs, const int32& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE int64 SubValues<int64>(const int64& InLhs, const int64& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE uint8 SubValues<uint8>(const uint8& InLhs, const uint8& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE uint16 SubValues<uint16>(const uint16& InLhs, const uint16& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE uint32 SubValues<uint32>(const uint32& InLhs, const uint32& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE uint64 SubValues<uint64>(const uint64& InLhs, const uint64& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FVector SubValues<FVector>(const FVector& InLhs, const FVector& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FVector2D SubValues<FVector2D>(const FVector2D& InLhs, const FVector2D& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FVector4 SubValues<FVector4>(const FVector4& InLhs, const FVector4& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FColor SubValues<FColor>(const FColor& InLhs, const FColor& InRhs)
{
    return FColor((uint8)FMath::Max((int32)InLhs.R - (int32)InRhs.R, 0),
                  (uint8)FMath::Max((int32)InLhs.G - (int32)InRhs.G, 0),
                  (uint8)FMath::Max((int32)InLhs.B - (int32)InRhs.B, 0),
                  (uint8)FMath::Max((int32)InLhs.A - (int32)InRhs.A, 0));
}

template<>
VOXELMATEINLINE FLinearColor SubValues<FLinearColor>(const FLinearColor& InLhs, const FLinearColor& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FPackedNormal SubValues<FPackedNormal>(const FPackedNormal& InLhs, const FPackedNormal& InRhs)
{
    const FPackedNormal Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGB10A2N SubValues<FPackedRGB10A2N>(const FPackedRGB10A2N& InLhs, const FPackedRGB10A2N& InRhs)
{
    const FPackedRGB10A2N Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGBA16N SubValues<FPackedRGBA16N>(const FPackedRGBA16N& InLhs, const FPackedRGBA16N& InRhs)
{
    const FPackedRGBA16N Normal = FVector4(InLhs) - FVector4(InRhs);
    return Normal;
}

template<>
VOXELMATEINLINE FIntPoint SubValues<FIntPoint>(const FIntPoint& InLhs, const FIntPoint& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE FIntVector SubValues<FIntVector>(const FIntVector& InLhs, const FIntVector& InRhs)
{
    return InLhs - InRhs;
}

template<>
VOXELMATEINLINE bool AbsValue<bool>(const bool& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE double AbsValue<double>(const double& InValue)
{
    return FMath::Abs(InValue);
}

template<>
VOXELMATEINLINE float AbsValue<float>(const float& InValue)
{
    return FMath::Abs(InValue);
}

template<>
VOXELMATEINLINE int8 AbsValue<int8>(const int8& InValue)
{
    return FMath::Abs<int64>(InValue);
}

template<>
VOXELMATEINLINE int16 AbsValue<int16>(const int16& InValue)
{
    return FMath::Abs<int64>(InValue);
}

template<>
VOXELMATEINLINE int32 AbsValue<int32>(const int32& InValue)
{
    return FMath::Abs<int64>(InValue);
}

template<>
VOXELMATEINLINE int64 AbsValue<int64>(const int64& InValue)
{
    return FMath::Abs<int64>(InValue);
}

template<>
VOXELMATEINLINE uint8 AbsValue<uint8>(const uint8& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint16 AbsValue<uint16>(const uint16& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint32 AbsValue<uint32>(const uint32& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint64 AbsValue<uint64>(const uint64& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE FVector AbsValue<FVector>(const FVector& InValue)
{
    return InValue.GetAbs();
}

template<>
VOXELMATEINLINE FVector2D AbsValue<FVector2D>(const FVector2D& InValue)
{
    return InValue.GetAbs();
}

template<>
VOXELMATEINLINE FVector4 AbsValue<FVector4>(const FVector4& InValue)
{
    return FVector4(FMath::Abs(InValue.X), FMath::Abs(InValue.Y), FMath::Abs(InValue.Z), FMath::Abs(InValue.W));
}

template<>
VOXELMATEINLINE FColor AbsValue<FColor>(const FColor& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE FLinearColor AbsValue<FLinearColor>(const FLinearColor& InValue)
{
    return FVector4(FMath::Abs(InValue.R), FMath::Abs(InValue.G), FMath::Abs(InValue.B), FMath::Abs(InValue.A));
}

template<>
VOXELMATEINLINE FPackedNormal AbsValue<FPackedNormal>(const FPackedNormal& InValue)
{
    FPackedNormal Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGB10A2N AbsValue<FPackedRGB10A2N>(const FPackedRGB10A2N& InValue)
{
    FPackedRGB10A2N Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGBA16N AbsValue<FPackedRGBA16N>(const FPackedRGBA16N& InValue)
{
    FPackedRGBA16N Normal = AbsValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FIntPoint AbsValue<FIntPoint>(const FIntPoint& InValue)
{
    return FIntPoint(FMath::Abs(InValue.X), FMath::Abs(InValue.Y));
}

template<>
VOXELMATEINLINE FIntVector AbsValue<FIntVector>(const FIntVector& InValue)
{
    return FIntVector(FMath::Abs(InValue.X), FMath::Abs(InValue.Y), FMath::Abs(InValue.Z));
}

template<>
VOXELMATEINLINE bool AddFloat<bool>(const bool& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE double AddFloat<double>(const double& InValue, const float& InFloat)
{
    return InValue + (double)InFloat;
}

template<>
VOXELMATEINLINE float AddFloat<float>(const float& InValue, const float& InFloat)
{
    return InValue + InFloat;
}

template<>
VOXELMATEINLINE int8 AddFloat<int8>(const int8& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE int16 AddFloat<int16>(const int16& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE int32 AddFloat<int32>(const int32& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE int64 AddFloat<int64>(const int64& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint8 AddFloat<uint8>(const uint8& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint16 AddFloat<uint16>(const uint16& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint32 AddFloat<uint32>(const uint32& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint64 AddFloat<uint64>(const uint64& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE FVector AddFloat<FVector>(const FVector& InValue, const float& InFloat)
{
    return InValue + InFloat;
}

template<>
VOXELMATEINLINE FVector2D AddFloat<FVector2D>(const FVector2D& InValue, const float& InFloat)
{
    return InValue + InFloat;
}

template<>
VOXELMATEINLINE FVector4 AddFloat<FVector4>(const FVector4& InValue, const float& InFloat)
{
    return FVector4(InValue.X + InFloat, InValue.Y + InFloat, InValue.Z + InFloat, InValue.W + InFloat);
}

template<>
VOXELMATEINLINE FColor AddFloat<FColor>(const FColor& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE FLinearColor AddFloat<FLinearColor>(const FLinearColor& InValue, const float& InFloat)
{
    return FLinearColor(InValue.R + InFloat, InValue.G + InFloat, InValue.B + InFloat, InValue.A + InFloat);
}

template<>
VOXELMATEINLINE FPackedNormal AddFloat<FPackedNormal>(const FPackedNormal& InValue, const float& InFloat)
{
    const FPackedNormal Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGB10A2N AddFloat<FPackedRGB10A2N>(const FPackedRGB10A2N& InValue, const float& InFloat)
{
    const FPackedRGB10A2N Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGBA16N AddFloat<FPackedRGBA16N>(const FPackedRGBA16N& InValue, const float& InFloat)
{
    const FPackedRGBA16N Normal = AddFloat<FVector4>(FVector4(InValue), InFloat);
    return Normal;
}

template<>
VOXELMATEINLINE FIntPoint AddFloat<FIntPoint>(const FIntPoint& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE FIntVector AddFloat<FIntVector>(const FIntVector& InValue, const float& InFloat)
{
    return InValue;
}

template<>
VOXELMATEINLINE bool NegValue<bool>(const bool& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE double NegValue<double>(const double& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE float NegValue<float>(const float& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE int8 NegValue<int8>(const int8& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE int16 NegValue<int16>(const int16& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE int32 NegValue<int32>(const int32& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE int64 NegValue<int64>(const int64& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE uint8 NegValue<uint8>(const uint8& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint16 NegValue<uint16>(const uint16& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint32 NegValue<uint32>(const uint32& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE uint64 NegValue<uint64>(const uint64& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE FVector NegValue<FVector>(const FVector& InValue)
{
    return -InValue;
}

template<>
VOXELMATEINLINE FVector2D NegValue<FVector2D>(const FVector2D& InValue)
{
    return FVector2D(-InValue.X, -InValue.Y);
}

template<>
VOXELMATEINLINE FVector4 NegValue<FVector4>(const FVector4& InValue)
{
    return FVector4(-InValue.X, -InValue.Y, -InValue.Z, -InValue.W);
}

template<>
VOXELMATEINLINE FColor NegValue<FColor>(const FColor& InValue)
{
    return InValue;
}

template<>
VOXELMATEINLINE FLinearColor NegValue<FLinearColor>(const FLinearColor& InValue)
{
    return FLinearColor(-InValue.R, -InValue.G, -InValue.B, -InValue.A);
}

template<>
VOXELMATEINLINE FPackedNormal NegValue<FPackedNormal>(const FPackedNormal& InValue)
{
    const FPackedNormal Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGB10A2N NegValue<FPackedRGB10A2N>(const FPackedRGB10A2N& InValue)
{
    const FPackedRGB10A2N Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FPackedRGBA16N NegValue<FPackedRGBA16N>(const FPackedRGBA16N& InValue)
{
    const FPackedRGBA16N Normal = NegValue<FVector4>(FVector4(InValue));
    return Normal;
}

template<>
VOXELMATEINLINE FIntPoint NegValue<FIntPoint>(const FIntPoint& InValue)
{
    return FIntPoint(-InValue.X, -InValue.Y);
}

template<>
VOXELMATEINLINE FIntVector NegValue<FIntVector>(const FIntVector& InValue)
{
    return FIntVector(-InValue.X, -InValue.Y, -InValue.Z);
}

} //namespace VoxelMateGridTypes
