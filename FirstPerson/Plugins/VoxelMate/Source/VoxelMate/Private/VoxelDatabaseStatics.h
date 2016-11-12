#pragma once
#include "EngineGridTypes.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

namespace VoxelDatabaseStatics
{
    template<typename Type> FORCEINLINE CONSTEXPR int32 SizeOf();

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::Vec3d>()
    {
        //Size of the primitive type (double) * 3
        return sizeof(openvdb::Vec3d::ValueType)*openvdb::Vec3d::size;
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::Mat4d>()
    {
        //Size of the primitive type (double) * 4 * 4
        return sizeof(openvdb::math::Mat4d::ValueType)*openvdb::math::Mat4d::size*openvdb::math::Mat4d::size;
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::Coord>()
    {
        //Size of the private union mVec
        //typedef union { struct { Int32 mX, mY, mZ; }; Int32 mVec[3]; } CoordData;
        return sizeof(openvdb::Coord::mVec);
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::CoordBBox>()
    {
        //Size of the min and max components (each are openvdb::Coord)
        return 2 * SizeOf<openvdb::Coord>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::ScaleMap>()
    {
        //Size of the scale, voxel scale, inverse scale, inverse scale squared, and the inverse 2x-scale (each are openvdb::Vec3d)
        return 5 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::TranslationMap>()
    {
        //Size of the translation (is an openvdb::Vec3d)
        return SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::ScaleTranslateMap>()
    {
        //Size of the translation, scale, voxel scale, inverse scale, inverse scale squared, and inverse 2x-scale (each are openvdb::Vec3d)
        return 6 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::UniformScaleMap>()
    {
        //Size of the scale, voxel scale, inverse scale, inverse scale squared, and the inverse 2x-scale (each are openvdb::Vec3d)
        return 5 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::UniformScaleTranslateMap>()
    {
        //Size of the translation, scale, voxel scale, inverse scale, inverse scale squared, and inverse 2x-scale (each are openvdb::Vec3d)
        return 6 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::AffineMap>()
    {
        //Size of the 4d-matrix (is an openvdb::Mat4d)
        return SizeOf<openvdb::math::Mat4d>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::UnitaryMap>()
    {
        //Size of the affine map (is an openvdb::math::AffineMap)
        return SizeOf<openvdb::math::AffineMap>();
    }

    template<> FORCEINLINE CONSTEXPR int32 SizeOf<openvdb::math::NonlinearFrustumMap>()
    {
        //Size of the bounding box (is an openvdb::CoordBBox), taper and depth values (both are double), and size of the "second map" (openvdb::math::AffineMap), and size of the AffineMap type string minus null terminator
        return SizeOf<openvdb::CoordBBox>() +
            sizeof(double) * 2 +
            sizeof(double) +
            SizeOf<openvdb::math::AffineMap>() +
            sizeof(openvdb::math::AffineMap::mapType()) - 1;
    }

    namespace GridStatics
    {
        const extern FString HalfFloatTypenameSuffix;
        //const static FString HalfFloatTypenameSuffix = UTF8_TO_TCHAR(HALF_FLOAT_TYPENAME_SUFFIX);
        //extern const ANSICHAR* HALF_FLOAT_TYPENAME_SUFFIX; FFFFUUUUU can't get this to link from the anonymous namespace
        const extern FString MetaNameGridDisplayText; //TODO maybe make openvdb::Name copies of these
        const extern FString MetaNameGridClass;
        const extern FString MetaNameGridCreator;
        const extern FString MetaNameGridName;
        const extern FString MetaNameSaveHalfFloat;
        const extern FString MetaNameIsLocalSpace;
        const extern FString MetaNameVectorType;
        const extern FString MetaNameFileBBoxMin;
        const extern FString MetaNameFileBBoxMax;
        const extern FString MetaNameFileCompression;
        const extern FString MetaNameFileMemBytes;
    }

    namespace TransformMapStatics
    {
        static int32 SizeOfMap(const FString& TypeName)
        {
            int32 MapSize = 0;
            const openvdb::Name TypeStr = TCHAR_TO_UTF8(*TypeName);
            if (TypeStr == openvdb::math::ScaleMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::ScaleMap>();
            }
            else if (TypeStr == openvdb::math::TranslationMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::TranslationMap>();
            }
            else if (TypeStr == openvdb::math::ScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::ScaleTranslateMap>();
            }
            else if (TypeStr == openvdb::math::UniformScaleMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UniformScaleMap>();
            }
            else if (TypeStr == openvdb::math::UniformScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UniformScaleTranslateMap>();
            }
            else if (TypeStr == openvdb::math::AffineMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::AffineMap>();
            }
            else if (TypeStr == openvdb::math::UnitaryMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UnitaryMap>();
            }
            else if (TypeStr == openvdb::math::NonlinearFrustumMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::NonlinearFrustumMap>();
            }
            return MapSize;
        }
    }
}

namespace GridIOStatics
{
    enum
    {
        FileVersionRootNodeMap             = openvdb::OPENVDB_FILE_VERSION_ROOTNODE_MAP,
        FileVersionInternalNodeCompression = openvdb::OPENVDB_FILE_VERSION_INTERNALNODE_COMPRESSION,
        FileVersionSimplifiedGridTypename  = openvdb::OPENVDB_FILE_VERSION_SIMPLIFIED_GRID_TYPENAME,
        FileVersionGridInstancing          = openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING,
        FileVersionBoolLeafOptimization    = openvdb::OPENVDB_FILE_VERSION_BOOL_LEAF_OPTIMIZATION,
        FileVersionBoostUuid               = openvdb::OPENVDB_FILE_VERSION_BOOST_UUID,
        FileVersionNoGridMap               = openvdb::OPENVDB_FILE_VERSION_NO_GRIDMAP,
        FileVersionNewTransform            = openvdb::OPENVDB_FILE_VERSION_NEW_TRANSFORM,
        FileVersionSelectiveCompression    = openvdb::OPENVDB_FILE_VERSION_SELECTIVE_COMPRESSION,
        FileVersionFloatFrustumBbox        = openvdb::OPENVDB_FILE_VERSION_FLOAT_FRUSTUM_BBOX,
        FileVersionNodeMaskCompression     = openvdb::OPENVDB_FILE_VERSION_NODE_MASK_COMPRESSION,
        FileVersionBloscCompression        = openvdb::OPENVDB_FILE_VERSION_BLOSC_COMPRESSION,
        FileVersionPointIndexGrid          = openvdb::OPENVDB_FILE_VERSION_POINT_INDEX_GRID,
    };

    static const TCHAR* GetMapTypeName(const openvdb::Name& MapType)
    {
        if (MapType == openvdb::math::AffineMap::mapType())
        {
            return TEXT("FAffineMap");
        }
        else if (MapType == openvdb::math::UnitaryMap::mapType())
        {
            return TEXT("FUnitaryMap");
        }
        else if (MapType == openvdb::math::ScaleMap::mapType())
        {
            return TEXT("FScaleMap");
        }
        else if (MapType == openvdb::math::UniformScaleMap::mapType())
        {
            return TEXT("FUniformScaleMap");
        }
        else if (MapType == openvdb::math::TranslationMap::mapType())
        {
            return TEXT("FTranslationMap");
        }
        else if (MapType == openvdb::math::ScaleTranslateMap::mapType())
        {
            return TEXT("FScaleTranslationMap");
        }
        else if (MapType == openvdb::math::UniformScaleTranslateMap::mapType())
        {
            return TEXT("FUniformScaleTranslationMap");
        }
        else if (MapType == openvdb::math::NonlinearFrustumMap::mapType())
        {
            return TEXT("FNonlinearFrustumMap");
        }
        else
        {
            return TEXT("");
        }
    }

    static const ANSICHAR* GetMapTypeName(const FString& MapType)
    {
        openvdb::Name OpenVdbMapTypeName;
        if (MapType == TEXT("FAffineMap"))
        {
            return openvdb::math::AffineMap::mapType().c_str();
        }
        else if (MapType == TEXT("FUnitaryMap"))
        {
            return openvdb::math::UnitaryMap::mapType().c_str();
        }
        else if (MapType == TEXT("FScaleMap"))
        {
            return openvdb::math::ScaleMap::mapType().c_str();
        }
        else if (MapType == TEXT("FUniformScaleMap"))
        {
            return openvdb::math::UniformScaleMap::mapType().c_str();
        }
        else if (MapType == TEXT("FTranslationMap"))
        {
            return openvdb::math::TranslationMap::mapType().c_str();
        }
        else if (MapType == TEXT("FScaleTranslationMap"))
        {
            return openvdb::math::ScaleTranslateMap::mapType().c_str();
        }
        else if (MapType == TEXT("FUniformScaleTranslationMap"))
        {
            return openvdb::math::UniformScaleTranslateMap::mapType().c_str();
        }
        else if (MapType == TEXT("FNonlinearFrustumMap"))
        {
            return openvdb::math::NonlinearFrustumMap::mapType().c_str();
        }
        else
        {
            return "";
        }
    }
} //namespace VoxelDatabaseStatics

#pragma warning(pop)
