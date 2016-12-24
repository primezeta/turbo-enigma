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
			VoxelType VoxelValue;
            ValueSource.GetValue(Xyz.x(), Xyz.y(), Xyz.z(), VoxelValue);
            iter.modifyValue<ModifyValueOp<VoxelType>>(ModifyValueOp<VoxelType>(VoxelValue));
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

		TExtractSurfaceOp(GridType& Grid)
			: GridAcc(Grid.tree()), SurfaceValue(Grid.tree().background())
		{}

		VOXELMATEINLINE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());

			const openvdb::Coord Coord = iter.getCoord();
			ValueType Values[8] = {
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

			Values[0].SurfaceIntersection = InsideBits;
			iter.modifyValue<ModifyValueOp<ValueType>>(ModifyValueOp<ValueType>(ValueType(Values[0])));

			if (InsideBits == 0 || InsideBits == 255)
			{
				//Turn voxel off since it is outside the surface
				iter.setValueOff();
			}
		}

	private:
		const ValueType& SurfaceValue;
		AccessorType GridAcc;
	};

	template<typename GridType>
	class TExtractSurfaceOp<typename GridType::ValueOffIter, GridType>
	{
	public:
		typedef typename GridType::ValueOffIter IterType;
		typedef typename GridType::Accessor AccessorType;
		typedef typename GridType::TreeType::ValueType ValueType;

		TExtractSurfaceOp(GridType& Grid)
			: GridAcc(Grid.tree()), SurfaceValue(Grid.tree().background())
		{}

		VOXELMATEINLINE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());

			const openvdb::Coord Coord = iter.getCoord();
			ValueType Values[8] = {
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

			Values[0].SurfaceIntersection = InsideBits;
			iter.modifyValue<ModifyValueOp<ValueType>>(ModifyValueOp<ValueType>(ValueType(Values[0])));

			if (InsideBits > 0 && InsideBits < 255)
			{
				//Turn voxel on since it is on the surface
				iter.setActiveState(true);
			}
		}

	private:
		const ValueType& SurfaceValue;
		AccessorType GridAcc;
	};

	////Static helper function since FProcMeshVertex only has the default constructor
	//static FORCEINLINE FProcMeshVertex ConstructProcMeshVertex(FVector vtx, FVector normal, FProcMeshTangent tangent, FColor color, FVector2D uv)
	//{
	//	FProcMeshVertex ProcMeshVertex;
	//	ProcMeshVertex.Position = vtx;
	//	ProcMeshVertex.Normal = normal;
	//	ProcMeshVertex.Tangent = tangent;
	//	ProcMeshVertex.Color = color;
	//	ProcMeshVertex.UV0 = uv;
	//	return ProcMeshVertex;
	//}

	////Operator to mesh the previously extracted isosurface via the Marching Cubes algorithm
	//template <typename GridType>
	//class MarchingCubesMeshOp
	//{
	//public:
	//	typedef typename GridType::ValueOnIter IterType;
	//	typedef typename GridType::Accessor AccessorType;
	//	typedef typename GridType::TreeType::ValueType ValueType;

	//	MarchingCubesMeshOp(GridType& Grid, TArray<FProcMeshSection>& Buffers)
	//		: DataGrid(Grid), GridAcc(Grid.tree()), SectionBuffers(Buffers), SurfaceValue(Grid.tree().background())
	//	{
	//		VertexCriticalSections.AddDefaulted(SectionBuffers.Num());
	//		TriCriticalSections.AddDefaulted(SectionBuffers.Num());
	//	}

	//	VOXELMATEINLINE void operator()(const IterType& iter)
	//	{
	//		check(iter.isVoxelValue());

	//		const openvdb::Coord Coord = iter.getCoord();

	//		const openvdb::math::Transform &Xform = DataGrid.transform();
	//		const openvdb::Coord p[8] =
	//		{
	//			Coord,
	//			Coord.offsetBy(0, 1, 0),
	//			Coord.offsetBy(1, 1, 0),
	//			Coord.offsetBy(1, 0, 0),
	//			Coord.offsetBy(0, 0, 1),
	//			Coord.offsetBy(0, 1, 1),
	//			Coord.offsetBy(1, 1, 1),
	//			Coord.offsetBy(1, 0, 1),
	//		};
	//		const ValueType val[8] =
	//		{
	//			GridAcc.getValue(p[0]),
	//			GridAcc.getValue(p[1]),
	//			GridAcc.getValue(p[2]),
	//			GridAcc.getValue(p[3]),
	//			GridAcc.getValue(p[4]),
	//			GridAcc.getValue(p[5]),
	//			GridAcc.getValue(p[6]),
	//			GridAcc.getValue(p[7]),
	//		};
	//		const openvdb::Vec3d vec[8] =
	//		{
	//			Xform.indexToWorld(p[0]),
	//			Xform.indexToWorld(p[1]),
	//			Xform.indexToWorld(p[2]),
	//			Xform.indexToWorld(p[3]),
	//			Xform.indexToWorld(p[4]),
	//			Xform.indexToWorld(p[5]),
	//			Xform.indexToWorld(p[6]),
	//			Xform.indexToWorld(p[7]),
	//		};
	//		//const openvdb::Vec3f grad[8] =
	//		//{
	//		//	GradAcc.getValue(p[0]),
	//		//	GradAcc.getValue(p[1]),
	//		//	GradAcc.getValue(p[2]),
	//		//	GradAcc.getValue(p[3]),
	//		//	GradAcc.getValue(p[4]),
	//		//	GradAcc.getValue(p[5]),
	//		//	GradAcc.getValue(p[6]),
	//		//	GradAcc.getValue(p[7]),
	//		//};
	//		const openvdb::Vec3f grad[8] =
	//		{
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//			openvdb::Vec3f(0.0f),
	//		};

	//		const uint8& InsideBits = val[0].SurfaceIntersection;
	//		const uint8& VoxelType = val[0].VoxelType;

	//		FProcMeshSection& MeshSection = SectionBuffers[VoxelType];
	//		TArray<FProcMeshVertex>& vertices = MeshSection.ProcVertexBuffer;
	//		TArray<int32>& polygons = MeshSection.ProcIndexBuffer;
	//		FBox& sectionBounds = MeshSection.SectionLocalBox;
	//		FCriticalSection& vtxCriticalSection = VertexCriticalSections[VoxelType];
	//		FCriticalSection& triCriticalSection = TriCriticalSections[VoxelType];

	//		//Find the vertices where the surface intersects the cube, always using the lower coord first
	//		int32 vertlist[12];
	//		if (MC_EdgeTable[InsideBits] & 1)
	//		{
	//			vertlist[0] = VertexInterp(vec[0], vec[1], data[0], data[1], p[0], p[1], grad[0], grad[1], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 2)
	//		{
	//			vertlist[1] = VertexInterp(vec[1], vec[2], data[1], data[2], p[1], p[2], grad[1], grad[2], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 4)
	//		{
	//			vertlist[2] = VertexInterp(vec[2], vec[3], data[2], data[3], p[2], p[3], grad[2], grad[3], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 8)
	//		{
	//			vertlist[3] = VertexInterp(vec[3], vec[0], data[3], data[0], p[3], p[0], grad[3], grad[0], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 16)
	//		{
	//			vertlist[4] = VertexInterp(vec[4], vec[5], data[4], data[5], p[4], p[5], grad[4], grad[5], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 32)
	//		{
	//			vertlist[5] = VertexInterp(vec[5], vec[6], data[5], data[6], p[5], p[6], grad[5], grad[6], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 64)
	//		{
	//			vertlist[6] = VertexInterp(vec[6], vec[7], data[6], data[7], p[6], p[7], grad[6], grad[7], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 128)
	//		{
	//			vertlist[7] = VertexInterp(vec[7], vec[4], data[7], data[4], p[7], p[4], grad[7], grad[4], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 256)
	//		{
	//			vertlist[8] = VertexInterp(vec[0], vec[4], data[0], data[4], p[0], p[4], grad[0], grad[4], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 512)
	//		{
	//			vertlist[9] = VertexInterp(vec[1], vec[5], data[1], data[5], p[1], p[5], grad[1], grad[5], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 1024)
	//		{
	//			vertlist[10] = VertexInterp(vec[2], vec[6], data[2], data[6], p[2], p[6], grad[2], grad[6], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}
	//		if (MC_EdgeTable[InsideBits] & 2048)
	//		{
	//			vertlist[11] = VertexInterp(vec[3], vec[7], data[3], data[7], p[3], p[7], grad[3], grad[7], SurfaceValue, vtxCriticalSection, vertices, sectionBounds);
	//		}

	//		//Add the polygons and each of their backfaces
	//		for (int32 i = 0; MC_TriTable[InsideBits][i].s != -1; i += 3)
	//		{
	//			check(i > -1 && i < 16);
	//			check(MC_TriTable[InsideBits][i].s < 12);
	//			check(MC_TriTable[InsideBits][i + 1].s > -1 && MC_TriTable[InsideBits][i + 1].s < 12);
	//			check(MC_TriTable[InsideBits][i + 2].s > -1 && MC_TriTable[InsideBits][i + 2].s < 12);

	//			const int32 idxsFrontFace[3] = {
	//				vertlist[MC_TriTable[InsideBits][i].u],
	//				vertlist[MC_TriTable[InsideBits][i + 1].u],
	//				vertlist[MC_TriTable[InsideBits][i + 2].u]
	//			};
	//			const int32 idxsBackFace[3] = {
	//				idxsFrontFace[2] + 1,
	//				idxsFrontFace[1] + 1,
	//				idxsFrontFace[0] + 1
	//			};
	//			check(idxsFrontFace[0] != idxsFrontFace[1] && idxsFrontFace[0] != idxsFrontFace[2] && idxsFrontFace[1] != idxsFrontFace[2]);

	//			//Create the front face and back face polygons
	//			const FVector surfaceNormal = FVector::CrossProduct(vertices[idxsFrontFace[1]].Position - vertices[idxsFrontFace[0]].Position, vertices[idxsFrontFace[2]].Position - vertices[idxsFrontFace[0]].Position);
	//			{
	//				FScopeLock lock(&vtxCriticalSection);
	//				vertices[idxsFrontFace[0]].Normal += surfaceNormal;
	//				vertices[idxsFrontFace[1]].Normal += surfaceNormal;
	//				vertices[idxsFrontFace[2]].Normal += surfaceNormal;
	//				vertices[idxsBackFace[0]].Normal -= surfaceNormal;
	//				vertices[idxsBackFace[1]].Normal -= surfaceNormal;
	//				vertices[idxsBackFace[2]].Normal -= surfaceNormal;
	//			}
	//			{
	//				FScopeLock lock(&triCriticalSection);
	//				polygons.Add(idxsFrontFace[0]);
	//				polygons.Add(idxsFrontFace[1]);
	//				polygons.Add(idxsFrontFace[2]);
	//				polygons.Add(idxsBackFace[0]);
	//				polygons.Add(idxsBackFace[1]);
	//				polygons.Add(idxsBackFace[2]);
	//			}
	//		}
	//	}

	//	static int32 VertexInterp(const openvdb::Vec3d& vec1,
	//		const openvdb::Vec3d& vec2,
	//		const ValueType& valp1,
	//		const ValueType& valp2,
	//		const openvdb::Coord& c1,
	//		const openvdb::Coord& c2,
	//		const openvdb::Vec3f& g1,
	//		const openvdb::Vec3f& g2,
	//		const ValueType& surfaceValue,
	//		FCriticalSection& criticalSection,
	//		TArray<FProcMeshVertex>& vertices,
	//		FBox& sectionBounds)
	//	{
	//		//TODO: Try Gram-Schmidt orthogonalization for tangents? (modifies the tangent to definitely be orthogonal to the normal):
	//		//tangent -= normal * tangent.dot( normal );
	//		//tangent.normalize();
	//		int32 outVertex = -1;
	//		if (openvdb::math::isApproxEqual(valp1.Value, surfaceValue.Value) || openvdb::math::isApproxEqual(valp1.Value, valp2.Value))
	//		{
	//			FScopeLock lock(&criticalSection);
	//			outVertex = (int32)vertices.Add(ConstructProcMeshVertex(
	//				FVector(vec1.x(), vec1.y(), vec1.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(g1.x(), g1.y(), g1.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			vertices.Add(ConstructProcMeshVertex(
	//				FVector(vec1.x(), vec1.y(), vec1.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(-g1.x(), -g1.y(), -g1.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			sectionBounds += vertices[outVertex].Position;
	//		}
	//		else if (openvdb::math::isApproxEqual(valp2.Value, surfaceValue.Value))
	//		{
	//			FScopeLock lock(&criticalSection);
	//			outVertex = (int32)vertices.Add(ConstructProcMeshVertex(
	//				FVector(vec2.x(), vec2.y(), vec2.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(g2.x(), g2.y(), g2.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			vertices.Add(ConstructProcMeshVertex(
	//				FVector(vec2.x(), vec2.y(), vec2.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(g2.x(), g2.y(), g2.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			sectionBounds += vertices[outVertex].Position;
	//		}
	//		else
	//		{
	//			FScopeLock lock(&criticalSection);
	//			const float lerpScale = (surfaceValue.Value - valp1.Value) / (valp2.Value - valp1.Value);
	//			const openvdb::Vec3d vtx = vec1 + (lerpScale*(vec2 - vec1));
	//			const openvdb::Vec3d grad = g1; //TODO: Figure out tangents on lerp'd vertices
	//			outVertex = (int32)vertices.Add(ConstructProcMeshVertex(
	//				FVector(vtx.x(), vtx.y(), vtx.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(grad.x(), grad.y(), grad.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			vertices.Add(ConstructProcMeshVertex(
	//				FVector(vtx.x(), vtx.y(), vtx.z()),
	//				FVector::ZeroVector, //initialize normal to 0
	//				FProcMeshTangent(-grad.x(), -grad.y(), -grad.z()),
	//				FColor(), //TODO colors
	//				FVector2D::ZeroVector //TODO uvs
	//			));
	//			sectionBounds += vertices[outVertex].Position;
	//		}
	//		check(outVertex > -1);
	//		return outVertex;
	//	}

	//	//openvdb::Grid<openvdb::Vec3fTree> Gradient;

	//private:
	//	GridType DataGrid;
	//	AccessorType GridAcc;
	//	const ValueType& SurfaceValue;
	//	TArray<FProcMeshSection>& SectionBuffers;
	//	TArray<FCriticalSection> VertexCriticalSections;
	//	TArray<FCriticalSection> TriCriticalSections;
	//};

	////Operator to mesh a cube at each active voxel from a previously extracted isosurface
	//template <typename TreeType, typename IterType>
	//class CubesMeshOp
	//{
	//public:
	//	typedef typename openvdb::Grid<TreeType> GridType;
	//	typedef typename GridType::Ptr GridTypePtr;
	//	typedef typename GridType::Accessor AccessorType;
	//	typedef typename GridType::ConstAccessor CAccessorType;
	//	typedef typename GridType::ValueType ValueType;
	//	typedef typename IterType SourceIterType;

	//	CubesMeshOp(GridType &grid, TArray<FProcMeshSection> &sectionBuffers)
	//		: Grid(grid), SectionBuffers(sectionBuffers)
	//	{
	//	}

	//	OPENVDB_INLINE void operator()(const SourceIterType& iter)
	//	{
	//		check(iter.isVoxelValue() && !iter.isTileValue());
	//		openvdb::CoordBBox bbox;
	//		const bool hasVoxelVolume = iter.getBoundingBox(bbox) && bbox.hasVolume() && bbox.volume() == 1;
	//		check(hasVoxelVolume);

	//		//Mesh the voxel as a simple cube with 6 equal sized quads
	//		bbox.expand(bbox.min(), 2);
	//		const openvdb::BBoxd worldBBox = Grid.transform().indexToWorld(bbox);
	//		const openvdb::Vec3d vtxs[8] = {
	//			worldBBox.min(),
	//			openvdb::Vec3d(worldBBox.max().x(), worldBBox.min().y(), worldBBox.min().z()),
	//			openvdb::Vec3d(worldBBox.min().x(), worldBBox.min().y(), worldBBox.max().z()),
	//			openvdb::Vec3d(worldBBox.max().x(), worldBBox.min().y(), worldBBox.max().z()),
	//			openvdb::Vec3d(worldBBox.min().x(), worldBBox.max().y(), worldBBox.min().z()),
	//			openvdb::Vec3d(worldBBox.min().x(), worldBBox.max().y(), worldBBox.max().z()),
	//			openvdb::Vec3d(worldBBox.max().x(), worldBBox.max().y(), worldBBox.min().z()),
	//			worldBBox.max()
	//		};

	//		const ValueType value = iter.getValue();
	//		const int32 idx = (int32)value.VoxelType;
	//		check(value.VoxelType != EVoxelType::VOXEL_NONE && idx > -1 && idx < FVoxelData::VOXEL_TYPE_COUNT);
	//		FProcMeshSection &meshSection = SectionBuffers[idx];
	//		TArray<FProcMeshVertex> &vertices = meshSection.ProcVertexBuffer;
	//		TArray<int32> &polygons = meshSection.ProcIndexBuffer;
	//		FBox &sectionBounds = meshSection.SectionLocalBox;
	//		FCriticalSection &criticalSection = CriticalSections[idx];
	//		{
	//			FScopeLock lock(&criticalSection);
	//			//Add polygons each with unique vertices (vertex indices added clockwise order on each quad face)
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.25f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.25f)
	//			)));
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());
	//			sectionBounds += FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z());
	//			sectionBounds += FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.25f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(0.0f, -1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			sectionBounds += FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z());
	//			sectionBounds += FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z());
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f, 0.75f)
	//			)));
	//			sectionBounds += FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z());
	//			sectionBounds += FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z());
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z()),
	//				FVector(1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.5f)
	//			)));
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());
	//			sectionBounds += FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z());
	//			sectionBounds += FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 1.0f)
	//			)));
	//			sectionBounds += FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z());
	//			sectionBounds += FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z());
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 1.0f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 1.0f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z()),
	//				FVector(0.0f, 1.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.75f)
	//			)));
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());
	//			sectionBounds += FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z());
	//			sectionBounds += FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(0.0f, 0.75f)
	//			)));
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());
	//			sectionBounds += FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z());
	//			sectionBounds += FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(0.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(0.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(-1.0f, 0.0f, 0.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			sectionBounds += FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z());
	//			sectionBounds += FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z());
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.25f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.0f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(0.0f, 0.0f, 1.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.0f)
	//			)));
	//			sectionBounds += FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z());
	//			sectionBounds += FVector(vtxs[5].x(), vtxs[5].y(), vtxs[5].z());
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.0f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.25f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z()),
	//				FVector(0.0f, 0.0f, 1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.25f)
	//			)));
	//			sectionBounds += FVector(vtxs[7].x(), vtxs[7].y(), vtxs[7].z());
	//			sectionBounds += FVector(vtxs[3].x(), vtxs[3].y(), vtxs[3].z());
	//			sectionBounds += FVector(vtxs[2].x(), vtxs[2].y(), vtxs[2].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.5f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.75f)
	//			)));
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());
	//			sectionBounds += FVector(vtxs[1].x(), vtxs[1].y(), vtxs[1].z());
	//			sectionBounds += FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z());

	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(2.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.75f)
	//			)));
	//			polygons.Add(vertices.Add(constructFProcMeshVertex(
	//				FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z()),
	//				FVector(0.0f, 0.0f, -1.0f),
	//				FProcMeshTangent(1.0f, 0.0f, 0.0f),
	//				FColor(), //TODO colors
	//				FVector2D(1.0f / 3.0f, 0.5f)
	//			)));
	//			sectionBounds += FVector(vtxs[6].x(), vtxs[6].y(), vtxs[6].z());
	//			sectionBounds += FVector(vtxs[4].x(), vtxs[4].y(), vtxs[4].z());
	//			sectionBounds += FVector(vtxs[0].x(), vtxs[0].y(), vtxs[0].z());
	//		}
	//	}

	//protected:
	//	GridType &Grid;
	//	TArray<FProcMeshSection> &SectionBuffers;
	//	FCriticalSection CriticalSections[FVoxelData::VOXEL_TYPE_COUNT];
	//};
}
