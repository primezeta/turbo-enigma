#pragma once
#include "Platform.h"
#include "EngineMinimal.h" //TODO: Engine.h is included just for UENUM, UCLASS, etc. Find exact header for these defines
#include "Math/Vector2D.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/IntVector.h"
#include "IntVector2.h"

#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>

struct FVoxelDatabaseHeader
{
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoxelDatabaseHeader& VoxelDatabaseHeaderHeader)
    {
        //TODO
        return Ar;
    }
};

//TODO: Reorganize .h files and move this definition
FORCEINLINE FArchive& operator<<(FArchive& Ar, openvdb::Name& Name)
{
    if (Ar.IsLoading())
    {
        int32 SaveNum = -1;
        Ar << SaveNum;

        if (SaveNum < 0)
        {
            //Archive is corrupted
            Ar.ArIsError = 1;
            Ar.ArIsCriticalError = 1;
            //UE_LOG(LogNetSerialization, Error, TEXT("Archive is corrupted")); TODO openvdb::Name logging
            return Ar;
        }

        if (SaveNum > Name.max_size())
        {
            //Number of characters too large
            Ar.ArIsError = 1;
            Ar.ArIsCriticalError = 1;
            //UE_LOG(LogNetSerialization, Error, TEXT("String is too large")); TODO openvdb::Name logging
            return Ar;
        }

        auto MaxSerializeSize = Ar.GetMaxSerializeSize();
        // Protect against network packets allocating too much memory
        if ((MaxSerializeSize > 0) && (SaveNum > MaxSerializeSize))
        {
            Ar.ArIsError = 1;
            Ar.ArIsCriticalError = 1;
            //UE_LOG(LogNetSerialization, Error, TEXT("String is too large")); TODO openvdb::Name logging
            return Ar;
        }

        // Resize the string only if it passes the above tests to prevent rogue packets from crashing
        Name.clear();
        if (SaveNum)
        {
            Name.resize(SaveNum);
            if (SaveNum > 1)
            {
                //Serialized string contains characters. Read them in
                Ar.Serialize((void*)&Name[0], SaveNum);
            }
        }
    }
    else
    {
        int32 SaveNum = Name.size();
        Ar << SaveNum;
        if (SaveNum)
        {
            Ar.Serialize((void*)&Name[0], SaveNum);
        }
    }
    return Ar;
}

//////////////////////////////////
//Type definitions
typedef openvdb::Name FGridName;
typedef openvdb::Index FIndex;
typedef std::map<FGridName, int32> FNameCountMap;
typedef std::set<FGridName> FUniqueNameSet;
typedef FNameCountMap::iterator FGridDescriptorNameMapIter;
typedef FNameCountMap::const_iterator FGridDescriptorNameMapCIter;
typedef std::map<std::string, boost::any> FAuxDataMap;
typedef openvdb::PointIndex32 FPointIndex32;
typedef openvdb::PointIndex64 FPointIndex64;
typedef openvdb::math::AffineMap FTransformAffineMap;
typedef openvdb::math::UnitaryMap FTransformUnitaryMap;
typedef openvdb::math::ScaleMap FTransformScaleMap;
typedef openvdb::math::UniformScaleMap FTransformUniformScaleMap;
typedef openvdb::math::TranslationMap FTransformTranslationMap;
typedef openvdb::math::ScaleTranslateMap FTransformScaleTranslateMap;
typedef openvdb::math::UniformScaleTranslateMap FTransformUniformScaleTranslateMap;
typedef openvdb::math::NonlinearFrustumMap FTransformNonlinearFrustumMap;

template<typename DataType>
using FTree3 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>>>;

template<typename DataType>
using FTree4 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>, 5>>>;

template<typename DataType>
using FTree5 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>, 5>, 6>>>;

//openvdb::io::COMPRESS_NONE
//openvdb::io::COMPRESS_ZIP
//openvdb::io::COMPRESS_ACTIVE_MASK
//openvdb::io::COMPRESS_BLOSC
//NOTE: Must ensure these types remain aligned. Unfortunately must set UEnum values to literal
UENUM(BlueprintType)
enum class EGridCompression : uint8
{
    GridCompressionNone       = 0x0,
    GridCompressionZip        = 0x1,
    GridCompressionActiveMask = 0x2,
    GridCompressionBlosc      = 0x4
};

inline bool GridCompressionFlagsAreInRange(uint8 GridCompressionFlags)
{
    const uint8 AllCompressionFlagBits =
        (uint8)EGridCompression::GridCompressionNone       &
        (uint8)EGridCompression::GridCompressionZip        &
        (uint8)EGridCompression::GridCompressionActiveMask &
        (uint8)EGridCompression::GridCompressionBlosc;
    return (GridCompressionFlags & (~AllCompressionFlagBits)) == 0;
}

inline bool GridCompressionIsNone(uint8 GridCompressionFlags)
{
    return GridCompressionFlags == (uint8)EGridCompression::GridCompressionNone;
}

inline bool GridCompressionIsZip(uint8 GridCompressionFlags)
{
    return (GridCompressionFlags & (uint8)EGridCompression::GridCompressionZip) > 0;
}

inline bool GridCompressionHasActiveMask(uint8 GridCompressionFlags)
{
    return (GridCompressionFlags & (uint8)EGridCompression::GridCompressionActiveMask) > 0;
}

inline bool GridCompressionHasBlosc (uint8 GridCompressionFlags)
{
    return (GridCompressionFlags & (uint8)EGridCompression::GridCompressionBlosc) > 0;
}

inline uint8 SetGridCompressionFlagOn(uint8 GridCompressionFlags, EGridCompression CompressionFlag)
{
    return GridCompressionFlags | (uint8)CompressionFlag;
}

inline uint8 SetGridCompressionFlagOff(uint8 GridCompressionFlags, EGridCompression CompressionFlag)
{
    return GridCompressionFlags & (~(uint8)CompressionFlag);
}

//openvdb::GRID_UNKNOWN
//openvdb::GRID_LEVEL_SET
//openvdb::GRID_FOG_VOLUME
//openvdb::GRID_STAGGERED
//NOTE: Must ensure these types remain aligned. Unfortunately must set UEnum values to literal
UENUM(BlueprintType)
enum class EGridClass : uint8
{
    GridClassUnknown = 0,
    GridClassLevelSet,
    GridClassFogVolume,
    GridClassStaggered
};

//openvdb::VEC_INVARIANT
//openvdb::VEC_COVARIANT
//openvdb::VEC_COVARIANT_NORMALIZE
//openvdb::VEC_CONTRAVARIANT_RELATIVE
//openvdb::VEC_CONTRAVARIANT_ABSOLUTE
//NOTE: Must ensure these types remain aligned. Unfortunately must set UEnum values to literal
UENUM(BlueprintType)
enum class EVectorTypeClass : uint8
{
    VectorTypeInvariant = 0,
    VectorTypeCovariant,
    VectorTypeCovariantNormalize,
    VectorTypeContravariantRelative,
    VectorTypeContravariantAbsolute
};

UENUM(BlueprintType)
enum class ETreeType : uint8
{
    Tree3,
    Tree4,
    Tree5
};

namespace GridExceptions
{
    typedef openvdb::ArithmeticError FArithmeticError;
    typedef openvdb::IllegalValueException FIllegalValueError;
    typedef openvdb::IndexError FIndexError;
    typedef openvdb::IoError FIoError;
    typedef openvdb::KeyError FKeyError;
    typedef openvdb::LookupError FLookupError;
    typedef openvdb::NotImplementedError FNotImplementedError;
    typedef openvdb::ReferenceError FReferenceError;
    typedef openvdb::RuntimeError FRuntimeError;
    typedef openvdb::TypeError FTypeError;
    typedef openvdb::ValueError FValueError;
}

template<typename Type> inline FString VoxelDatabaseTypeName()
{
    return FString(UTF8_TO_TCHAR(openvdb::typeNameAsString<Type>()));
}

//////////////////////////////////
//Functions required by OpenVDB for custom grid types

//////////////////////////////////
//FVector2D
inline FVector2D Abs(const FVector2D& Vec);
inline std::ostream& operator<<(std::ostream& os, const FVector2D& Vec);

template<> inline FVector2D openvdb::zeroVal<FVector2D>()
{
    return FVector2D::ZeroVector;
}

template<> inline const char* openvdb::typeNameAsString<FVector2D>()
{
    return "Vector 2D";
}

//////////////////////////////////
//FVector
inline FVector Abs(const FVector &Vec);
inline std::ostream& operator<<(std::ostream& os, const FVector& Vec);
inline bool operator<(const FVector& Lhs, const FVector& Rhs);
inline bool operator>(const FVector& Lhs, const FVector& Rhs);

template<> inline FVector openvdb::zeroVal<FVector>()
{
    return FVector::ZeroVector;
}

template<> inline const char* openvdb::typeNameAsString<FVector>()
{
    return "Vector 3D";
}

//////////////////////////////////
//FVector4
inline FVector4 Abs(const FVector4 &Vec);
inline std::ostream& operator<<(std::ostream& os, const FVector4& Vec);
inline FVector4 operator+(const FVector4& Vec, const float& Val);
inline bool operator<(const FVector4& Lhs, const FVector4& Rhs);
inline bool operator>(const FVector4& Lhs, const FVector4& Rhs);

template<> inline FVector4 openvdb::zeroVal<FVector4>()
{
    return FVector4(ForceInitToZero);
}

template<> inline const char* openvdb::typeNameAsString<FVector4>()
{
    return "Vector 4D";
}

//////////////////////////////////
//FIntVector2
inline FIntVector2 Abs(const FIntVector2 &IVec);
inline std::ostream& operator<<(std::ostream& os, const FIntVector2& Vec);
inline FIntVector2 operator+(const FIntVector2& Vec, const float& Val);
inline FIntVector2 operator-(const FIntVector2& Vec);
inline bool operator<(const FIntVector2& Lhs, const FIntVector2& Rhs);
inline bool operator>(const FIntVector2& Lhs, const FIntVector2& Rhs);

template<> inline FIntVector2 openvdb::zeroVal<FIntVector2>()
{
    return FIntVector2::ZeroValue;
}

template<> inline const char* openvdb::typeNameAsString<FIntVector2>()
{
    return "IntVector 2D";
}

//////////////////////////////////
//FIntVector
inline FIntVector Abs(const FIntVector &IVec);
inline std::ostream& operator<<(std::ostream& os, const FIntVector& Vec);
inline FIntVector operator+(const FIntVector& Vec, const float& Val);
inline FIntVector operator-(const FIntVector& Vec);
inline bool operator<(const FIntVector& Lhs, const FIntVector& Rhs);
inline bool operator>(const FIntVector& Lhs, const FIntVector& Rhs);

template<> inline FIntVector openvdb::zeroVal<FIntVector>()
{
    return FIntVector::ZeroValue;
}

template<> inline const char* openvdb::typeNameAsString<FIntVector>()
{
    return "IntVector 3D";
}

//////////////////////////////////
//FIntVector4
inline FIntVector4 Abs(const FIntVector4 &IVec);
inline std::ostream& operator<<(std::ostream& os, const FIntVector4& Vec);
inline FIntVector4 operator+(const FIntVector4& Vec, const float& Val);
inline FIntVector4 operator+(const FIntVector4& Lhs, const FIntVector4& Rhs);
inline FIntVector4 operator-(const FIntVector4& Lhs, const FIntVector4& Rhs);
inline FIntVector4 operator-(const FIntVector4& Vec);
inline bool operator<(const FIntVector4& Lhs, const FIntVector4& Rhs);
inline bool operator>(const FIntVector4& Lhs, const FIntVector4& Rhs);

template<> inline FIntVector4 openvdb::zeroVal<FIntVector4>()
{
    return FIntVector4(ForceInitToZero);
}

template<> inline const char* openvdb::typeNameAsString<FIntVector4>()
{
    return "IntVector 4D";
}

//////////////////////////////////
//FUintVector4
inline FUintVector4 Abs(const FUintVector4 &UVec);
inline std::ostream& operator<<(std::ostream& os, const FUintVector4& Vec);
inline FUintVector4 operator+(const FUintVector4& Vec, const float& Val);
inline FUintVector4 operator+(const FUintVector4& Lhs, const FUintVector4& Rhs);
inline FUintVector4 operator-(const FUintVector4& Lhs, const FUintVector4& Rhs);
inline FUintVector4 operator-(const FUintVector4& Vec);
inline bool operator<(const FUintVector4& Lhs, const FUintVector4& Rhs);
inline bool operator>(const FUintVector4& Lhs, const FUintVector4& Rhs);

template<> inline FUintVector4 openvdb::zeroVal<FUintVector4>()
{
    return FUintVector4(ForceInitToZero);
}

template<> inline const char* openvdb::typeNameAsString<FUintVector4>()
{
    return "UIntVector 4D";
}

//////////////////////////////////
//FMatrix
inline std::ostream& operator<<(std::ostream& os, const FMatrix& Mat);

template<> inline FMatrix openvdb::zeroVal<FMatrix>()
{
    return FMatrix(ForceInitToZero);
}

template<> inline const char* openvdb::typeNameAsString<FMatrix>()
{
    return "Matrix 4x4";
}

//////////////////////////////////
//FTransformAffineMap
template<> inline const char* openvdb::typeNameAsString<FTransformAffineMap>()
{
    return "Affine";
}

//////////////////////////////////
//FTransformUnitaryMap
template<> inline const char* openvdb::typeNameAsString<FTransformUnitaryMap>()
{
    return "Unitary";
}

//////////////////////////////////
//FTransformScaleMap
template<> inline const char* openvdb::typeNameAsString<FTransformScaleMap>()
{
    return "Scaling";
}

//////////////////////////////////
//FTransformUniformScaleMap
template<> inline const char* openvdb::typeNameAsString<FTransformUniformScaleMap>()
{
    return "Uniform Scaling";
}

//////////////////////////////////
//FTransformTranslationMap
template<> inline const char* openvdb::typeNameAsString<FTransformTranslationMap>()
{
    return "Translation";
}

//////////////////////////////////
//FTransformScaleTranslateMap
template<> inline const char* openvdb::typeNameAsString<FTransformScaleTranslateMap>()
{
    return "Scale and Translation";
}

//////////////////////////////////
//FTransformUniformScaleTranslateMap
template<> inline const char* openvdb::typeNameAsString<FTransformUniformScaleTranslateMap>()
{
    return "Uniform Scaling and Translation";
}

//////////////////////////////////
//FTransformNonlinearFrustumMap
template<> inline const char* openvdb::typeNameAsString<FTransformNonlinearFrustumMap>()
{
    return "Nonlinear Frustum";
}
