#pragma once
#include "EngineMinimal.h"
#include "EngineGridTypes.h"

#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <openvdb/io/io.h>

namespace VoxelDatabaseStatics
{
    template<typename ValueType> FORCEINLINE int32 SizeOf()
    {
        return sizeof(ValueType);
    }

    template<> FORCEINLINE int32 SizeOf<openvdb::Vec3d>()
    {
        //Size of the primitive type (double) * 3
        return sizeof(openvdb::Vec3d::ValueType)*openvdb::Vec3d::size;
    }

    template<> FORCEINLINE int32 SizeOf<openvdb::math::Mat4d>()
    {
        //Size of the primitive type (double) * 4 * 4
        return sizeof(openvdb::math::Mat4d::ValueType)*openvdb::math::Mat4d::size*openvdb::math::Mat4d::size;
    }

    template<> FORCEINLINE int32 SizeOf<openvdb::Coord>()
    {
        //Size of the private union mVec
        //typedef union { struct { Int32 mX, mY, mZ; }; Int32 mVec[3]; } CoordData;
        return sizeof(openvdb::Coord::mVec);
    }

    template<> FORCEINLINE int32 SizeOf<openvdb::CoordBBox>()
    {
        //Size of the min and max components (each are openvdb::Coord)
        return 2 * SizeOf<openvdb::Coord>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformScaleMap>()
    {
        //Size of the scale, voxel scale, inverse scale, inverse scale squared, and the inverse 2x-scale (each are openvdb::Vec3d)
        return 5 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformTranslationMap>()
    {
        //Size of the translation (is an openvdb::Vec3d)
        return SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformScaleTranslateMap>()
    {
        //Size of the translation, scale, voxel scale, inverse scale, inverse scale squared, and inverse 2x-scale (each are openvdb::Vec3d)
        return 6 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformUniformScaleMap>()
    {
        //Size of the scale, voxel scale, inverse scale, inverse scale squared, and the inverse 2x-scale (each are openvdb::Vec3d)
        return 5 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformUniformScaleTranslateMap>()
    {
        //Size of the translation, scale, voxel scale, inverse scale, inverse scale squared, and inverse 2x-scale (each are openvdb::Vec3d)
        return 6 * SizeOf<openvdb::Vec3d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformAffineMap>()
    {
        //Size of the 4d-matrix (is an openvdb::Mat4d)
        return SizeOf<openvdb::math::Mat4d>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformUnitaryMap>()
    {
        //Size of the affine map (is an openvdb::math::AffineMap)
        return SizeOf<FTransformAffineMap>();
    }

    template<> FORCEINLINE int32 SizeOf<FTransformNonlinearFrustumMap>()
    {
        //Size of the bounding box (is an openvdb::CoordBBox), taper and depth values (both are double), and size of the "second map" (openvdb::math::AffineMap), and size of the AffineMap type string minus null terminator
        return SizeOf<openvdb::CoordBBox>() + 2 * sizeof(FTransformNonlinearFrustumMap::mTaper) + sizeof(FTransformNonlinearFrustumMap::mDepth) + SizeOf<FTransformAffineMap>() + sizeof(FTransformAffineMap::mapType()) - 1;
    }

    struct GridStatics : private openvdb::GridBase
    {
        const static openvdb::Name MetaNameGridClass;
        const static openvdb::Name MetaNameGridCreator;
        const static openvdb::Name MetaNameGridName;
        const static openvdb::Name MetaNameSaveHalfFloat;
        const static openvdb::Name MetaNameIsLocalSpace;
        const static openvdb::Name MetaNameVectorType;
        const static openvdb::Name MetaNameFileBBoxMin;
        const static openvdb::Name MetaNameFileBBoxMax;
        const static openvdb::Name MetaNameFileCompression;
        const static openvdb::Name MetaNameFileMemBytes;

        static void RegisterGrid(const openvdb::Name& TypeName, openvdb::GridBase::GridFactory CreateFunction)
        {
            openvdb::GridBase::registerGrid(TypeName, CreateFunction);
        }

        static void UnregisterGrid(const openvdb::Name& TypeName)
        {
            openvdb::GridBase::unregisterGrid(TypeName);
        }

    protected:
        //Protected constructor to prevent instantiation
        GridStatics() {}
    };

    struct TransformMapStatics
    {
        static inline int32 SizeOfMap(const openvdb::Name& TypeName)
        {
            int32 MapSize = 0;
            if (TypeName == FTransformScaleMap::mapType())
            {
                MapSize = SizeOf<FTransformScaleMap>();
            }
            else if (TypeName == FTransformTranslationMap::mapType())
            {
                MapSize = SizeOf<FTransformTranslationMap>();
            }
            else if (TypeName == FTransformScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<FTransformScaleTranslateMap>();
            }
            else if (TypeName == FTransformUniformScaleMap::mapType())
            {
                MapSize = SizeOf<FTransformUniformScaleMap>();
            }
            else if (TypeName == FTransformUniformScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<FTransformUniformScaleTranslateMap>();
            }
            else if (TypeName == FTransformAffineMap::mapType())
            {
                MapSize = SizeOf<FTransformAffineMap>();
            }
            else if (TypeName == FTransformUnitaryMap::mapType())
            {
                MapSize = SizeOf<FTransformUnitaryMap>();
            }
            else if (TypeName == FTransformNonlinearFrustumMap::mapType())
            {
                MapSize = SizeOf<FTransformNonlinearFrustumMap>();
            }
            return MapSize;
        }

    protected:
        //Protected constructor to prevent instantiation
        TransformMapStatics() {}
    };
};

//extern const char* HALF_FLOAT_TYPENAME_SUFFIX; FFFFUUUUU can't get this to link from the anonymous namespace
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

    const static openvdb::Name HalfFloatTypenameSuffix = "_HalfFloat";
    //const static openvdb::Name HalfFloatTypenameSuffix = HALF_FLOAT_TYPENAME_SUFFIX;;

    static uint32 GetFormatVersion(std::ios_base& ios)
    {
        return openvdb::io::getFormatVersion(ios);
    }

    static void SetDataCompression(std::ios_base& ios, uint32 compressionFlags)
    {
        openvdb::io::setDataCompression(ios, compressionFlags);
    }

    static void SetWriteGridStatsMetadata(std::ios_base& ios, bool areGridStatsWritten)
    {
        openvdb::io::setWriteGridStatsMetadata(ios, areGridStatsWritten);
    }

    static uint32 GetDataCompression(std::ios_base& ios)
    {
        return openvdb::io::getDataCompression(ios);
    }

    static openvdb::Name CompressionToString(uint32 compressionFlags)
    {
        return openvdb::io::compressionToString(compressionFlags);
    }
}
