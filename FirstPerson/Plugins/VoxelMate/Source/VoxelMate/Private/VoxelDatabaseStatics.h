#pragma once
#include "Runtime/Core/Public/Math/Axis.h"
#include <openvdb/openvdb.h>

namespace VoxelDatabaseStatics
{
	template<typename T>
	FORCEINLINE CONSTEXPR void CheckSizeTypeRangeU(const SIZE_T& WideValue, const T& NarrowValue, const T& NarrowMax)
	{
		check(WideValue <= (SIZE_T)NarrowMax);
		check(static_cast<T>(WideValue) < NarrowValue);
	}

	template<typename T>
	FORCEINLINE CONSTEXPR void CheckSizeTypeRangeU(const SIZE_T& WideValue, const T& NarrowMax)
	{
		check(WideValue <= (SIZE_T)NarrowMax);
	}

	template<typename T>
	FORCEINLINE CONSTEXPR void CheckSizeTypeRangeS(const SSIZE_T& WideValue, const T& NarrowValue, const T& NarrowMin, const T& NarrowMax)
	{
		check(WideValue >= (SSIZE_T)NarrowMin);
		check(WideValue <= (SSIZE_T)NarrowMax);
		check(static_cast<T>(WideValue) < NarrowValue);
	}

	template<typename T>
	FORCEINLINE CONSTEXPR void CheckSizeTypeRangeS(const SSIZE_T& WideValue, const T& NarrowMin, const T& NarrowMax)
	{
		check(WideValue >= (SSIZE_T)NarrowMin);
		check(WideValue <= (SSIZE_T)NarrowMax);
	}

    template<typename Type> CONSTEXPR int32 SizeOf()
	{
		static_assert(false, "SizeOf not implemented");
	}

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
        FORCEINLINE int32 SizeOfMap(const openvdb::Name& TypeName)
        {
            int32 MapSize = 0;
            if (TypeName == openvdb::math::ScaleMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::ScaleMap>();
            }
            else if (TypeName == openvdb::math::TranslationMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::TranslationMap>();
            }
            else if (TypeName == openvdb::math::ScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::ScaleTranslateMap>();
            }
            else if (TypeName == openvdb::math::UniformScaleMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UniformScaleMap>();
            }
            else if (TypeName == openvdb::math::UniformScaleTranslateMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UniformScaleTranslateMap>();
            }
            else if (TypeName == openvdb::math::AffineMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::AffineMap>();
            }
            else if (TypeName == openvdb::math::UnitaryMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::UnitaryMap>();
            }
            else if (TypeName == openvdb::math::NonlinearFrustumMap::mapType())
            {
                MapSize = SizeOf<openvdb::math::NonlinearFrustumMap>();
            }
            return MapSize;
        }
    }

    enum
    {
        FileVersionRootNodeMap = openvdb::OPENVDB_FILE_VERSION_ROOTNODE_MAP,
        FileVersionInternalNodeCompression = openvdb::OPENVDB_FILE_VERSION_INTERNALNODE_COMPRESSION,
        FileVersionSimplifiedGridTypename = openvdb::OPENVDB_FILE_VERSION_SIMPLIFIED_GRID_TYPENAME,
        FileVersionGridInstancing = openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING,
        FileVersionBoolLeafOptimization = openvdb::OPENVDB_FILE_VERSION_BOOL_LEAF_OPTIMIZATION,
        FileVersionBoostUuid = openvdb::OPENVDB_FILE_VERSION_BOOST_UUID,
        FileVersionNoGridMap = openvdb::OPENVDB_FILE_VERSION_NO_GRIDMAP,
        FileVersionNewTransform = openvdb::OPENVDB_FILE_VERSION_NEW_TRANSFORM,
        FileVersionSelectiveCompression = openvdb::OPENVDB_FILE_VERSION_SELECTIVE_COMPRESSION,
        FileVersionFloatFrustumBbox = openvdb::OPENVDB_FILE_VERSION_FLOAT_FRUSTUM_BBOX,
        FileVersionNodeMaskCompression = openvdb::OPENVDB_FILE_VERSION_NODE_MASK_COMPRESSION,
        FileVersionBloscCompression = openvdb::OPENVDB_FILE_VERSION_BLOSC_COMPRESSION,
        FileVersionPointIndexGrid = openvdb::OPENVDB_FILE_VERSION_POINT_INDEX_GRID,
    };

    template<typename CoordinateTransformType>
	void SetGridCoordinateTransform(openvdb::GridBase &Grid, const CoordinateTransformType& CoordinateTransform)
    {
        static_assert(false, "SetGridCoordinateTransform not implemented");
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FAffineCoordinateTransform>(openvdb::GridBase &Grid, const FAffineCoordinateTransform& CoordinateTransform)
    {
        const FVector4 &Col0 = CoordinateTransform.Matrix.GetColumn(0);
        const FVector4 &Col1 = CoordinateTransform.Matrix.GetColumn(1);
        const FVector4 &Col2 = CoordinateTransform.Matrix.GetColumn(2);
        const openvdb::math::Mat3d AffineMatrix(
            Col0.X, Col1.X, Col2.X,
            Col0.Y, Col1.Y, Col2.Y,
            Col0.Z, Col1.Z, Col2.Z
        );
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::AffineMap(AffineMatrix)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FUnitaryCoordinateTransform>(openvdb::GridBase &Grid, const FUnitaryCoordinateTransform& CoordinateTransform)
    {
        FVector Axis = FVector::ZeroVector;
        float Angle = 0.0f;
        CoordinateTransform.Quat.ToAxisAndAngle(Axis, Angle);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UnitaryMap(openvdb::Vec3d(Axis.X, Axis.Y, Axis.Z), Angle)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FScaleCoordinateTransform>(openvdb::GridBase &Grid, const FScaleCoordinateTransform& CoordinateTransform)
    {
        const openvdb::Vec3d Scale(CoordinateTransform.ScaleVec.X, CoordinateTransform.ScaleVec.Y, CoordinateTransform.ScaleVec.Z);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleMap(Scale)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(openvdb::GridBase &Grid, const FUniformScaleCoordinateTransform& CoordinateTransform)
    {
        const float &Scale(CoordinateTransform.ScaleValue);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleMap(Scale)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FTranslationCoordinateTransform& CoordinateTransform)
    {
        const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::TranslationMap(Translation)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FScaleTranslationCoordinateTransform& CoordinateTransform)
    {
        const openvdb::Vec3d Scale(CoordinateTransform.ScaleVec.X, CoordinateTransform.ScaleVec.Y, CoordinateTransform.ScaleVec.Z);
        const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleTranslateMap(Scale, Translation)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FUniformScaleTranslationCoordinateTransform& CoordinateTransform)
    {
        const float &Scale = CoordinateTransform.ScaleValue;
        const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleTranslateMap(Scale, Translation)))));
    }

    template<>
    FORCEINLINE void SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(openvdb::GridBase &Grid, const FNonlinearFrustumCoordinateTransform& CoordinateTransform)
    {
        const openvdb::Vec3d Box0(CoordinateTransform.Box.Min.X, CoordinateTransform.Box.Min.Y, CoordinateTransform.Box.Min.Z);
        const openvdb::Vec3d Box1(CoordinateTransform.Box.Max.X, CoordinateTransform.Box.Max.Y, CoordinateTransform.Box.Max.Z);
        const openvdb::BBoxd BoundingBox(Box0 < Box1 ? Box0 : Box1, Box0 > Box1 ? Box0 : Box1);
        const float &Taper = CoordinateTransform.Taper;
        const float &Depth = CoordinateTransform.Depth;
        Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::NonlinearFrustumMap(BoundingBox, Taper, Depth)))));
    }

    FORCEINLINE openvdb::math::Axis GetAxisType(const EAxis::Type& Axis)
    {
        const openvdb::math::Axis AxisType =
            Axis == EAxis::X ? openvdb::math::Axis::X_AXIS :
            Axis == EAxis::Y ? openvdb::math::Axis::Y_AXIS :
            openvdb::math::Axis::Z_AXIS;
        return AxisType;
    }
}

namespace VoxelDatabaseUtil
{
    template<typename ValueType>
    struct ModifyValueOp
    {
        const ValueType& InValue;

        ModifyValueOp(const ValueType& Value)
            : InValue(Value)
        {}

        VOXELMATEINLINE void operator()(ValueType& OutValue) const
        {
            OutValue = InValue;
        }
    };

    struct ModifyActiveStateOp
    {
        const bool& InActiveState;

        ModifyActiveStateOp(const bool& ActiveState)
            : InActiveState(ActiveState)
        {}

        VOXELMATEINLINE void operator()(bool& OutActiveState) const
        {
            OutActiveState = InActiveState;
        }
    };

    template<EVoxelIterator Iter, typename GridType>
	struct VoxelIteratorAdaptor;

    template<typename GridType>
    struct VoxelIteratorAdaptor<EVoxelIterator::InactiveVoxelsIter, GridType>
    {
        typedef typename GridType::ValueOffIter Type;
        VOXELMATEINLINE static Type SelectIter(GridType& Grid)
        {
            return Grid.beginValueOff();
        }
    };

    template<typename GridType>
    struct VoxelIteratorAdaptor<EVoxelIterator::ActiveVoxelsIter, GridType>
    {
        typedef typename GridType::ValueOnIter Type;
        VOXELMATEINLINE static Type SelectIter(GridType& Grid)
        {
            return Grid.beginValueOn();
        }
    };

    template<typename GridType>
    struct VoxelIteratorAdaptor<EVoxelIterator::AllVoxelsIter, GridType>
    {
        typedef typename GridType::ValueAllIter Type;
        VOXELMATEINLINE static Type SelectIter(GridType& Grid)
        {
            return Grid.beginValueAll();
        }
    };

    template<typename IterT, typename VoxelT, typename ValueSourceT>
    struct TSetValuesOp
    {
        typedef typename VoxelT VoxelType;
        typedef typename VoxelT::ValueType ValueType;
        typedef typename IterT IterType;
        typedef typename ValueSourceT ValueSourceType;

        const ValueSourceType &ValueSource;
        openvdb::math::Transform &CoordinateTransform;

        TSetValuesOp(const ValueSourceType &InValueSource, openvdb::math::Transform &Transform)
            : ValueSource(InValueSource), CoordinateTransform(Transform)
        {}

        VOXELMATEINLINE void operator()(const IterType& iter) const
        {
            check(iter.isVoxelValue());
            const openvdb::Coord &Coord = iter.getCoord();
            const openvdb::Vec3d &Xyz = CoordinateTransform.indexToWorld(Coord);
            ValueType Value;
            ValueSource.GetValue(Xyz.x(), Xyz.y(), Xyz.z(), Value);
            iter.modifyValue<ModifyValueOp<VoxelType>>(ModifyValueOp<VoxelType>(VoxelType(Value)));
        }
    };

	template<typename IterType, typename GridType>
	class TExtractSurfaceOp;

	template<typename GridType>
	class TExtractSurfaceOp<typename GridType::ValueOnIter, GridType>
	{
	public:
		typedef typename GridType::ValueOnIter IterType;
		typedef typename GridType::Accessor AccessorType;
		typedef typename GridType::TreeType::ValueType ValueType;

		TExtractSurfaceOp(GridType &Grid)
			: GridAcc(Grid.tree()), SurfaceValue(Grid.tree().background())
		{}

		VOXELMATEINLINE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());

			const openvdb::Coord Coord = iter.getCoord();
			const ValueType Values[8] = {
				GridAcc.getValue(Coord),
				GridAcc.getValue(Coord.offsetBy(0, 1, 0)),
				GridAcc.getValue(Coord.offsetBy(1, 1, 0)),
				GridAcc.getValue(Coord.offsetBy(1, 0, 0)),
				GridAcc.getValue(Coord.offsetBy(0, 0, 1)),
				GridAcc.getValue(Coord.offsetBy(0, 1, 1)),
				GridAcc.getValue(Coord.offsetBy(1, 1, 1)),
				GridAcc.getValue(Coord.offsetBy(1, 0, 1)),
			};

			//Flag a vertex as inside the surface if the data value is less than the surface data value
			uint8 InsideBits = 0;
			if (Values[0] < SurfaceValue) { InsideBits |= 1; }
			if (Values[1] < SurfaceValue) { InsideBits |= 2; }
			if (Values[2] < SurfaceValue) { InsideBits |= 4; }
			if (Values[3] < SurfaceValue) { InsideBits |= 8; }
			if (Values[4] < SurfaceValue) { InsideBits |= 16; }
			if (Values[5] < SurfaceValue) { InsideBits |= 32; }
			if (Values[6] < SurfaceValue) { InsideBits |= 64; }
			if (Values[7] < SurfaceValue) { InsideBits |= 128; }

			if (InsideBits == 0 || InsideBits == 255)
			{
				//Turn voxel off since it is outside the surface
				iter.setValueOff();
			}
		}

	private:
		const ValueType &SurfaceValue;
		AccessorType GridAcc;
	};

	template<typename GridType>
	class TExtractSurfaceOp<typename GridType::ValueOffIter, GridType>
	{
	public:
		typedef typename GridType::ValueOffIter IterType;
		typedef typename GridType::Accessor AccessorType;
		typedef typename GridType::TreeType::ValueType ValueType;

		TExtractSurfaceOp(GridType &Grid)
			: GridAcc(Grid.tree()), SurfaceValue(Grid.tree().background())
		{}

		VOXELMATEINLINE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());

			const openvdb::Coord Coord = iter.getCoord();
			const ValueType Values[8] = {
				GridAcc.getValue(Coord),
				GridAcc.getValue(Coord.offsetBy(0, 1, 0)),
				GridAcc.getValue(Coord.offsetBy(1, 1, 0)),
				GridAcc.getValue(Coord.offsetBy(1, 0, 0)),
				GridAcc.getValue(Coord.offsetBy(0, 0, 1)),
				GridAcc.getValue(Coord.offsetBy(0, 1, 1)),
				GridAcc.getValue(Coord.offsetBy(1, 1, 1)),
				GridAcc.getValue(Coord.offsetBy(1, 0, 1)),
			};

			//Flag a vertex as inside the surface if the data value is less than the surface data value
			uint8 InsideBits = 0;
			if (Values[0] < SurfaceValue) { InsideBits |= 1; }
			if (Values[1] < SurfaceValue) { InsideBits |= 2; }
			if (Values[2] < SurfaceValue) { InsideBits |= 4; }
			if (Values[3] < SurfaceValue) { InsideBits |= 8; }
			if (Values[4] < SurfaceValue) { InsideBits |= 16; }
			if (Values[5] < SurfaceValue) { InsideBits |= 32; }
			if (Values[6] < SurfaceValue) { InsideBits |= 64; }
			if (Values[7] < SurfaceValue) { InsideBits |= 128; }

			if (InsideBits > 0 && InsideBits < 255)
			{
				//Turn voxel on since it is on the surface
				iter.setActiveState(true);
			}
		}

	private:
		const ValueType &SurfaceValue;
		AccessorType GridAcc;
	};
}
