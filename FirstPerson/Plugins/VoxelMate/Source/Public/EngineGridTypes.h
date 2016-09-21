#pragma once
#include "Platform.h"
#include "Engine.h" //TODO: Engine.h is included just for UENUM, UCLASS, etc. Find exact header for these defines
#include "Math/Vector2D.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/IntVector.h"
#include "IntVector2.h"
#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>

#define TO_GRID_DATABASE_STRING(fstring) TCHAR_TO_UTF8(*fstring)
#define FROM_GRID_DATABASE_STRING(grid_database_string) UTF8_TO_TCHAR(grid_database_string.c_str())

//////////////////////////////////
//Forward declarations
class FGridDescriptor;
template<typename MapType> class FTransformMap;

//TODO: Reorganize .h files and move this definition
class FGridDatabaseString : public std::string
{
public:
    FGridDatabaseString()
        : std::string()
    {}

    FGridDatabaseString(const std::string& str)
        : std::string(str)
    {}

    FGridDatabaseString(const char* cstr)
        : std::string(cstr)
    {}

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGridDatabaseString& GridDatabaseString)
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
                //UE_LOG(LogNetSerialization, Error, TEXT("Archive is corrupted")); TODO FGridDatabaseString logging
                return Ar;
            }

            if (SaveNum > GridDatabaseString.max_size())
            {
                //Number of characters too large
                Ar.ArIsError = 1;
                Ar.ArIsCriticalError = 1;
                //UE_LOG(LogNetSerialization, Error, TEXT("String is too large")); TODO FGridDatabaseString logging
                return Ar;
            }

            auto MaxSerializeSize = Ar.GetMaxSerializeSize();
            // Protect against network packets allocating too much memory
            if ((MaxSerializeSize > 0) && (SaveNum > MaxSerializeSize))
            {
                Ar.ArIsError = 1;
                Ar.ArIsCriticalError = 1;
                //UE_LOG(LogNetSerialization, Error, TEXT("String is too large")); TODO FGridDatabaseString logging
                return Ar;
            }

            // Resize the string only if it passes the above tests to prevent rogue packets from crashing
            GridDatabaseString.clear();
            if (SaveNum)
            {
                GridDatabaseString.resize(SaveNum);
                if (SaveNum > 1)
                {
                    //Serialized string contains characters. Read them in
                    Ar.Serialize((void*)&GridDatabaseString[0], SaveNum);
                }
            }
        }
        else
        {
            int32 SaveNum = GridDatabaseString.size();
            Ar << SaveNum;
            if (SaveNum)
            {
                Ar.Serialize((void*)&GridDatabaseString[0], SaveNum);
            }
        }
        return Ar;
    }
};

//////////////////////////////////
//Type definitions
typedef openvdb::Name FGridName;
typedef openvdb::Index FIndex;
typedef std::multimap<FGridName, boost::shared_ptr<FGridDescriptor>> FGridDescriptorNameMap;
typedef std::map<FGridName, int32> FNameCountMap;
typedef std::set<FGridName> FUniqueNameSet;
typedef std::map<const openvdb::TreeBase*, FGridDescriptor> FTreeMap;
typedef FTreeMap::iterator FTreeMapIter;
typedef FGridDescriptorNameMap::iterator FGridDescriptorNameMapIter;
typedef FGridDescriptorNameMap::const_iterator FGridDescriptorNameMapCIter;
typedef openvdb::MetaMap FMetaMap;
typedef std::map<std::string, boost::any> FAuxDataMap;
typedef openvdb::PointIndex32 FPointIndex32;
typedef openvdb::PointIndex64 FPointIndex64;
typedef FTransformMap<openvdb::math::AffineMap> FTransformAffineMap;
typedef FTransformMap<openvdb::math::UnitaryMap> FTransformUnitaryMap;
typedef FTransformMap<openvdb::math::ScaleMap> FTransformScaleMap;
typedef FTransformMap<openvdb::math::UniformScaleMap> FTransformUniformScaleMap;
typedef FTransformMap<openvdb::math::TranslationMap> FTransformTranslationMap;
typedef FTransformMap<openvdb::math::ScaleTranslateMap> FTransformScaleTranslateMap;
typedef FTransformMap<openvdb::math::UniformScaleTranslateMap> FTransformUniformScaleTranslateMap;
typedef FTransformMap<openvdb::math::NonlinearFrustumMap> FTransformNonlinearFrustumMap;

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
    VectorInvariant = 0,
    VectorCovariant,
    VectorCovariantNormalize,
    VectorContravariantRelative,
    VectorContravariantAbsolute
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

namespace openvdb
{
    template<typename Type> inline const char* typeNameAsDisplayString()
    {
        return typeid(Type).name();
    }
}

template<typename Type> inline FString GridTypeName()
{
    return FString(UTF8_TO_TCHAR(openvdb::typeNameAsString<Type>()));
}

template<typename Type> inline FString GridTypeNameDisplay()
{
    return FString(UTF8_TO_TCHAR(openvdb::typeNameAsDisplayString<Type>()));
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
    return "fvector2d";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FVector2D>()
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
    return "fvector";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FVector>()
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
    return "fvector4";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FVector4>()
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
    return "fintvector2";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FIntVector2>()
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
    return "fintvector";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FIntVector>()
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
    return "fintvector4";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FIntVector4>()
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
    return "fuintvector4";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FUintVector4>()
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
    return "fmatrix";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FMatrix>()
{
    return "Matrix 4x4";
}

//////////////////////////////////
//FPointIndex32
template<> inline const char* openvdb::typeNameAsDisplayString<FPointIndex32>()
{
    return "Point Index32";
}

//////////////////////////////////
//FPointIndex64
template<> inline const char* openvdb::typeNameAsDisplayString<FPointIndex64>()
{
    return "Point Index64";
}

//////////////////////////////////
//FGridDatabaseString
inline FGridDatabaseString Abs(const FGridDatabaseString &Str);
inline FGridDatabaseString operator-(const FGridDatabaseString& Lhs, const FGridDatabaseString& Rhs);
inline FGridDatabaseString operator-(const FGridDatabaseString& Str);

template<> inline FGridDatabaseString openvdb::zeroVal<FGridDatabaseString>()
{
    return FGridDatabaseString("");
}

template<> inline const char* openvdb::typeNameAsString<FGridDatabaseString>()
{
    return "fgriddatabasestring";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FGridDatabaseString>()
{
    return "Grid Database String";
}

//////////////////////////////////
//FTransformAffineMap
template<> inline const char* openvdb::typeNameAsString<FTransformAffineMap>()
{
    return "ftransformaffinemap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformAffineMap>()
{
    return "Affine";
}

//////////////////////////////////
//FTransformUnitaryMap
template<> inline const char* openvdb::typeNameAsString<FTransformUnitaryMap>()
{
    return "ftransformunitarymap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformUnitaryMap>()
{
    return "Unitary";
}

//////////////////////////////////
//FTransformScaleMap
template<> inline const char* openvdb::typeNameAsString<FTransformScaleMap>()
{
    return "ftransformscalemap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformScaleMap>()
{
    return "Scaling";
}

//////////////////////////////////
//FTransformUniformScaleMap
template<> inline const char* openvdb::typeNameAsString<FTransformUniformScaleMap>()
{
    return "ftransformuniformscalemap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformUniformScaleMap>()
{
    return "Uniform Scaling";
}

//////////////////////////////////
//FTransformTranslationMap
template<> inline const char* openvdb::typeNameAsString<FTransformTranslationMap>()
{
    return "ftransformtranslationmap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformTranslationMap>()
{
    return "Translation";
}

//////////////////////////////////
//FTransformScaleTranslateMap
template<> inline const char* openvdb::typeNameAsString<FTransformScaleTranslateMap>()
{
    return "ftransformscaletranslateMap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformScaleTranslateMap>()
{
    return "Scale and Translation";
}

//////////////////////////////////
//FTransformUniformScaleTranslateMap
template<> inline const char* openvdb::typeNameAsString<FTransformUniformScaleTranslateMap>()
{
    return "ftransformuniformscaletranslatemap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformUniformScaleTranslateMap>()
{
    return "Uniform Scaling and Translation";
}

//////////////////////////////////
//FTransformNonlinearFrustumMap
template<> inline const char* openvdb::typeNameAsString<FTransformNonlinearFrustumMap>()
{
    return "ftransformnonlinearfrustummap";
}

template<> inline const char* openvdb::typeNameAsDisplayString<FTransformNonlinearFrustumMap>()
{
    return "Nonlinear Frustum";
}
