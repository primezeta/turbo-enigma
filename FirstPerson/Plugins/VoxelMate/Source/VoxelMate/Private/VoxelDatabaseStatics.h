#pragma once
#include "Runtime/Core/Public/Math/Axis.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveMetaValue.h"
#include "ArchiveTransformMap.h"
#include "EngineGridTypes.h"
#include <openvdb/openvdb.h>
#include <openvdb/tools/ValueTransformer.h>
#include <openvdb/io/Archive.h>

namespace VoxelDatabaseUtil
{
	template<typename ValueType>
	struct ModifyValueOp
	{
		const ValueType& InValue;

		ModifyValueOp(const ValueType& Value)
			: InValue(Value)
		{}

		FORCEINLINE_DEBUGGABLE void operator()(ValueType& OutValue) const
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

		FORCEINLINE_DEBUGGABLE void operator()(bool& OutActiveState) const
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
		FORCEINLINE_DEBUGGABLE static Type SelectIter(GridType& Grid)
		{
			return Grid.beginValueOff();
		}
	};

	template<typename GridType>
	struct VoxelIteratorAdaptor<EVoxelIterator::ActiveVoxelsIter, GridType>
	{
		typedef typename GridType::ValueOnIter Type;
		FORCEINLINE_DEBUGGABLE static Type SelectIter(GridType& Grid)
		{
			return Grid.beginValueOn();
		}
	};

	template<typename GridType>
	struct VoxelIteratorAdaptor<EVoxelIterator::AllVoxelsIter, GridType>
	{
		typedef typename GridType::ValueAllIter Type;
		FORCEINLINE_DEBUGGABLE static Type SelectIter(GridType& Grid)
		{
			return Grid.beginValueAll();
		}
	};

	template<typename IterT, typename GridT>
	struct TServerUpdateChangedVoxelValuesOp
	{
		typedef typename GridT GridType;
		typedef typename GridT::ValueType VoxelType;
		typedef typename IterT IterType;

		AVoxelDatabase& ServerDatabase;
		const GridType& Grid;
		FIntVector GridId;

		TServerUpdateChangedVoxelValuesOp(AVoxelDatabase& InServerDatabase, const GridType& InGrid)
			: ServerDatabase(InServerDatabase), Grid(InGrid)
		{
			openvdb::TypedMetadata<FMetadataIntVector>::ConstPtr GridIdMeta = Grid.getMetadata<openvdb::TypedMetadata<FMetadataIntVector>>(TCHAR_TO_UTF8(*VoxelDatabaseStatics::MetaNameGridIndex));
			GridId = GridIdMeta->value().Value;
		}

		FORCEINLINE_DEBUGGABLE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());
			const openvdb::Coord &Coord = iter.getCoord();
			//ServerDatabase.ChangeVoxel(GridId, FIntVector(Coord.x(), Coord.y(), Coord.z()), iter.getValue(), iter.isValueOn());
		}
	};

	template<typename IterT, typename GridT>
	struct TSetValuesOp
	{
		typedef typename GridT GridType;
		typedef typename GridType::ValueType VoxelType;
		typedef typename IterT IterType;

		const AGridSource& GridSource;
		const GridType& Grid;

		TSetValuesOp(const AGridSource &InGridSource, const GridType& InGrid)
			: GridSource(InGridSource), Grid(InGrid)
		{}

		FORCEINLINE_DEBUGGABLE void operator()(const IterType& iter) const
		{
			check(iter.isVoxelValue());
			const openvdb::Coord &Coord = iter.getCoord();
			const openvdb::Vec3d &Xyz = Grid.transform().indexToWorld(Coord);
			VoxelType VoxelValue;
			GridSource.GetValue(Xyz.x(), Xyz.y(), Xyz.z(), VoxelValue);
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

		FORCEINLINE_DEBUGGABLE void operator()(const IterType& iter) const
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

		FORCEINLINE_DEBUGGABLE void operator()(const IterType& iter) const
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

	//	FORCEINLINE_DEBUGGABLE void operator()(const IterType& iter)
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

namespace VoxelDatabaseStatics
{
	const extern FString HalfFloatTypenameSuffix;
	//const static FString HalfFloatTypenameSuffix = UTF8_TO_TCHAR(HALF_FLOAT_TYPENAME_SUFFIX);
	//extern const ANSICHAR* HALF_FLOAT_TYPENAME_SUFFIX; FFFFUUUUU can't get this to link from the anonymous namespace
	const extern FString MetaNameGridValueSource;
	const extern FString MetaNameGridIndex;
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

	void StartupVoxelDatabase()
	{
		//TODO: Note that the following BLOSC related stuff is in openvdb::initialize
		//#ifdef OPENVDB_USE_BLOSC
		//    blosc_init();
		//    if (blosc_set_compressor("lz4") < 0) {
		//        OPENVDB_LOG_WARN("Blosc LZ4 compressor is unavailable");
		//    }
		//    /// @todo blosc_set_nthreads(int nthreads);
		//#endif
		FGridFactory::RegisterSupportedTypes();
		FMetaValueFactory::RegisterSupportedTypes();
		FTransformMapFactory::RegisterSupportedTypes();
	}

	void ShutdownVoxelDatabase()
	{
		FGridFactory::UnregisterSupportedTypes();
		FMetaValueFactory::UnregisterSupportedTypes();
		FTransformMapFactory::UnregisterSupportedTypes();
	}

	template<typename CoordinateTransformType>
	void SetGridCoordinateTransform(openvdb::GridBase &Grid, const CoordinateTransformType& CoordinateTransform)
	{
		static_assert(false, "SetGridCoordinateTransform not implemented");
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FAffineCoordinateTransform>(openvdb::GridBase &Grid, const FAffineCoordinateTransform& CoordinateTransform)
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
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FUnitaryCoordinateTransform>(openvdb::GridBase &Grid, const FUnitaryCoordinateTransform& CoordinateTransform)
	{
		FVector Axis = FVector::ZeroVector;
		float Angle = 0.0f;
		CoordinateTransform.Quat.ToAxisAndAngle(Axis, Angle);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UnitaryMap(openvdb::Vec3d(Axis.X, Axis.Y, Axis.Z), Angle)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FScaleCoordinateTransform>(openvdb::GridBase &Grid, const FScaleCoordinateTransform& CoordinateTransform)
	{
		const openvdb::Vec3d Scale(CoordinateTransform.ScaleVec.X, CoordinateTransform.ScaleVec.Y, CoordinateTransform.ScaleVec.Z);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleMap(Scale)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FUniformScaleCoordinateTransform>(openvdb::GridBase &Grid, const FUniformScaleCoordinateTransform& CoordinateTransform)
	{
		const float &Scale(CoordinateTransform.ScaleValue);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleMap(Scale)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FTranslationCoordinateTransform& CoordinateTransform)
	{
		const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::TranslationMap(Translation)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FScaleTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FScaleTranslationCoordinateTransform& CoordinateTransform)
	{
		const openvdb::Vec3d Scale(CoordinateTransform.ScaleVec.X, CoordinateTransform.ScaleVec.Y, CoordinateTransform.ScaleVec.Z);
		const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleTranslateMap(Scale, Translation)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FUniformScaleTranslationCoordinateTransform>(openvdb::GridBase &Grid, const FUniformScaleTranslationCoordinateTransform& CoordinateTransform)
	{
		const float &Scale = CoordinateTransform.ScaleValue;
		const openvdb::Vec3d Translation(CoordinateTransform.TranslationVec.X, CoordinateTransform.TranslationVec.Y, CoordinateTransform.TranslationVec.Z);
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleTranslateMap(Scale, Translation)))));
	}

	template<>
	FORCEINLINE_DEBUGGABLE void SetGridCoordinateTransform<FNonlinearFrustumCoordinateTransform>(openvdb::GridBase &Grid, const FNonlinearFrustumCoordinateTransform& CoordinateTransform)
	{
		const openvdb::Vec3d Box0(CoordinateTransform.Box.Min.X, CoordinateTransform.Box.Min.Y, CoordinateTransform.Box.Min.Z);
		const openvdb::Vec3d Box1(CoordinateTransform.Box.Max.X, CoordinateTransform.Box.Max.Y, CoordinateTransform.Box.Max.Z);
		const openvdb::BBoxd BoundingBox(Box0 < Box1 ? Box0 : Box1, Box0 > Box1 ? Box0 : Box1);
		const float &Taper = CoordinateTransform.Taper;
		const float &Depth = CoordinateTransform.Depth;
		Grid.setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::NonlinearFrustumMap(BoundingBox, Taper, Depth)))));
	}

	FORCEINLINE_DEBUGGABLE void CreateGridData(const FText& GridDisplayText, bool SaveFloatAsHalf, openvdb::GridBase::Ptr& OutGridPtr)
	{
		NET_LOG_STATIC(LogVoxelMate, "%s", *GridDisplayText.ToString());
		OutGridPtr = FGridFactory::Create<FVoxel>();
		OutGridPtr->setName(TCHAR_TO_UTF8(*GridDisplayText.ToString()));
		OutGridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
		//FMetaMapFactory::InsertGridMeta<FMetadataText>(*OutGridPtr, MetaNameGridDisplayText, FMetadataText(GridDisplayText));
	}

	//FORCEINLINE_DEBUGGABLE void CreateVoxelValuesFromSource(const openvdb::GridBase::Ptr& GridPtr, AGridSource& GridSource)
	//{
	//	const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
	//	if (TypedGridPtr)
	//	{
	//		const openvdb::Coord FillStart(0, 0, 0);
	//		const openvdb::Coord FillEnd(GridSource.GridSize.X - 1, GridSource.GridSize.Y - 1, GridSource.GridSize.Z - 1);
	//		const openvdb::CoordBBox FillBBox(FillStart, FillEnd);
	//		openvdb::TypedMetadata<FVoxel>::Ptr BackgroundValueMetadata = boost::static_pointer_cast<openvdb::TypedMetadata<FVoxel>>(TypedGridPtr->tree().getBackgroundValue());

	//		TypedGridPtr->fill(FillBBox, BackgroundValueMetadata->value(), true);
	//		TypedGridPtr->tree().voxelizeActiveTiles();

	//		FTranslationCoordinateTransform CoordTransform(GridSource.GridTranslation);
	//		SetGridCoordinateTransform<FTranslationCoordinateTransform>(*GridPtr, CoordTransform);

	//		GridPtr->insertMeta(TCHAR_TO_UTF8(*MetaNameGridIndex), openvdb::TypedMetadata<FMetadataIntVector>(FMetadataIntVector(GridSource.GridIndex)));

	//		typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<EVoxelIterator::ActiveVoxelsIter, GridType> IterAdaptor;
	//		typedef VoxelDatabaseUtil::TSetValuesOp<IterAdaptor::Type, GridType> OpType;

	//		OpType SetValuesOp(GridSource, *TypedGridPtr);
	//		openvdb::tools::foreach<IterAdaptor::Type, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), SetValuesOp);
	//	}
	//	else
	//	{
	//		//TODO log error (grid types mismatched)
	//		check(false); //TODO handle error
	//	}
	//}

	FORCEINLINE_DEBUGGABLE void ServerUpdateChangedVoxelValues(const openvdb::GridBase::Ptr& GridPtr, AVoxelDatabase& ServerDatabase)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<EVoxelIterator::AllVoxelsIter, GridType> IterAdaptor;
			typedef VoxelDatabaseUtil::TServerUpdateChangedVoxelValuesOp<IterAdaptor::Type, GridType> OpType;

			OpType UpdateValuesOp(ServerDatabase, *TypedGridPtr);
			openvdb::tools::foreach<IterAdaptor::Type, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), UpdateValuesOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	FORCEINLINE_DEBUGGABLE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const FVoxel& InVoxelValue)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::Accessor Accessor = TypedGridPtr->getAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
			Accessor.setValueOnly(Coord, InVoxelValue);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	FORCEINLINE_DEBUGGABLE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const FVoxel& InVoxelValue, const bool& InIsActive)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::Accessor Accessor = TypedGridPtr->getAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);

			if (InIsActive)
			{
				Accessor.setValueOn(Coord, InVoxelValue);
			}
			else
			{
				Accessor.setValueOff(Coord, InVoxelValue);
			}
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	FORCEINLINE_DEBUGGABLE void SetVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& VoxelIndexCoord, const bool& InIsActive)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::Accessor Accessor = TypedGridPtr->getAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
			Accessor.setActiveState(Coord, InIsActive);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	FORCEINLINE_DEBUGGABLE void ChangeVoxelValue(const openvdb::GridBase::Ptr& GridPtr, const FIntVector& IndexCoord, const FVoxel& Voxel, const bool& IsActive)
	{
		check(GridPtr != nullptr);
		SetVoxelValue(GridPtr, IndexCoord, Voxel, IsActive);
	}

	FORCEINLINE_DEBUGGABLE const FVoxel& GetVoxelValue(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord)
	{
		const GridType::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
			return Accessor.getValue(Coord);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}

		const static FVoxel Zero = FVoxel();
		return Zero;
	}

	FORCEINLINE_DEBUGGABLE const FVoxel& GetVoxelValue(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord, bool& OutIsVoxelActive)
	{
		const GridType::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
			OutIsVoxelActive = Accessor.isValueOn(Coord);
			return Accessor.getValue(Coord);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}

		OutIsVoxelActive = false;
		const static FVoxel Zero = FVoxel();
		return Zero;
	}

	FORCEINLINE_DEBUGGABLE void GetVoxelIsActive(const openvdb::GridBase::ConstPtr& GridPtr, const FIntVector& VoxelIndexCoord, bool& OutIsActive)
	{
		const GridType::ConstPtr TypedGridPtr = openvdb::gridConstPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			GridType::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
			const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
			OutIsActive = Accessor.isValueOn(Coord);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}

		OutIsActive = false;
	}

	template<EVoxelIterator VoxelIterType>
	FORCEINLINE_DEBUGGABLE void ExtractGridSurface(const openvdb::GridBase::Ptr& GridPtr)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<VoxelIterType, GridType> IterAdaptor;
			typedef VoxelDatabaseUtil::TExtractSurfaceOp<IterAdaptor::Type, GridType> OpType;

			OpType ExtractSurfaceOp(*TypedGridPtr);
			openvdb::tools::foreach<IterAdaptor::Type, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), ExtractSurfaceOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	template<EVoxelIterator VoxelIterType>
	FORCEINLINE_DEBUGGABLE void MeshGridSurface(const openvdb::GridBase::Ptr& GridPtr)
	{
		const GridType::Ptr TypedGridPtr = openvdb::gridPtrCast<GridType>(GridPtr);
		if (TypedGridPtr)
		{
			typedef VoxelDatabaseUtil::VoxelIteratorAdaptor<VoxelIterType, GridType> IterAdaptor;
			typedef VoxelDatabaseUtil::TExtractSurfaceOp<IterAdaptor::Type, GridType> OpType;

			OpType ExtractSurfaceOp(*TypedGridPtr);
			openvdb::tools::foreach<IterAdaptor::Type, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), ExtractSurfaceOp);
		}
		else
		{
			//TODO log error (grid types mismatched)
			check(false); //TODO handle error
		}
	}

	FORCEINLINE_DEBUGGABLE openvdb::math::Axis GetAxisType(const EAxis::Type& Axis)
	{
		const openvdb::math::Axis AxisType =
			Axis == EAxis::X ? openvdb::math::Axis::X_AXIS :
			Axis == EAxis::Y ? openvdb::math::Axis::Y_AXIS :
			openvdb::math::Axis::Z_AXIS;
		return AxisType;
	}

	FORCEINLINE_DEBUGGABLE void AddGridRotation(openvdb::GridBase &Grid, ETransformOp Op, float Radians, EAxis::Type Axis)
	{
		const openvdb::math::Axis RotationAxis = GetAxisType(Axis);
		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preRotate(Radians, RotationAxis);
		}
		else
		{
			Grid.transform().postRotate(Radians, RotationAxis);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridTranslation(openvdb::GridBase &Grid, ETransformOp Op, const FVector &InTranslation)
	{
		const openvdb::Vec3d Translation(InTranslation.X, InTranslation.Y, InTranslation.Z);

		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preTranslate(Translation);
		}
		else
		{
			Grid.transform().postTranslate(Translation);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridScale(openvdb::GridBase &Grid, ETransformOp Op, const FVector &InScale)
	{
		const openvdb::Vec3d Scale(InScale.X, InScale.Y, InScale.Z);

		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preScale(Scale);
		}
		else
		{
			Grid.transform().postScale(Scale);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridUniformScale(openvdb::GridBase &Grid, ETransformOp Op, float Scale)
	{
		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preScale(Scale);
		}
		else
		{
			Grid.transform().postScale(Scale);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridShear(openvdb::GridBase &Grid, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1)
	{
		const openvdb::math::Axis ShearAxis0 = GetAxisType(Axis0);
		const openvdb::math::Axis ShearAxis1 = GetAxisType(Axis1);

		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preShear(Shear, ShearAxis0, ShearAxis1);
		}
		else
		{
			Grid.transform().postShear(Shear, ShearAxis0, ShearAxis1);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridMatrix4dMultiply(openvdb::GridBase &Grid, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW)
	{
		const openvdb::math::Mat4d Matrix(
			InX.X, InX.Y, InX.Z, InX.W,
			InY.X, InY.Y, InY.Z, InY.W,
			InZ.X, InZ.Y, InZ.Z, InZ.W,
			InW.X, InW.Y, InW.Z, InW.W
		);

		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preMult(Matrix);
		}
		else
		{
			Grid.transform().postMult(Matrix);
		}
	}

	FORCEINLINE_DEBUGGABLE void AddGridMatrix3dMultiply(openvdb::GridBase &Grid, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ)
	{
		const openvdb::math::Mat3d Matrix(
			InX.X, InX.Y, InX.Z,
			InY.X, InY.Y, InY.Z,
			InZ.X, InZ.Y, InZ.Z
		);

		if (Op == ETransformOp::PreOp)
		{
			Grid.transform().preMult(Matrix);
		}
		else
		{
			Grid.transform().postMult(Matrix);
		}
	}

	FORCEINLINE_DEBUGGABLE void ReadGrid(FArchive& Ar, TMap<FIntVector, openvdb::GridBase::Ptr>& Grids)
	{
		check(Ar.IsLoading());

		FString GridName;
		Ar << GridName;

		FString GridTypeName;
		Ar << GridTypeName;

		openvdb::GridBase::Ptr GridPtr = FGridFactory::CreateType(GridTypeName);
		if (GridPtr != nullptr)
		{
			TArray<ANSICHAR> GridBytes;
			Ar << GridBytes;

			FStreamReader<ANSICHAR> GridReader(GridBytes, Ar.IsPersistent());
			GridPtr->readMeta(GridReader.GetStream());
			GridPtr->readTopology(GridReader.GetStream());
			GridPtr->readTransform(GridReader.GetStream());
			GridPtr->readBuffers(GridReader.GetStream());

			openvdb::TypedMetadata<FMetadataIntVector>::ConstPtr GridIdMeta = GridPtr->getMetadata<openvdb::TypedMetadata<FMetadataIntVector>>(TCHAR_TO_UTF8(*MetaNameGridIndex));
			Grids.Add(GridIdMeta->value().Value, GridPtr);
		}
		else
		{
			//Skip past the array portion
			int32 GridByteCount = 0;
			Ar << GridByteCount;

			const int64 CurrentPos = Ar.Tell();
			Ar.Seek(CurrentPos + (int64)GridByteCount);
		}
	}

	FORCEINLINE_DEBUGGABLE void WriteGrid(FArchive& Ar, const openvdb::GridBase::Ptr& GridPtr)
	{
		check(Ar.IsSaving());

		if (GridPtr != nullptr)
		{
			FString GridName = UTF8_TO_TCHAR(GridPtr->getName().c_str());
			Ar << GridName;

			FString GridTypeName = UTF8_TO_TCHAR(GridPtr->type().c_str());
			Ar << GridTypeName;

			TArray<ANSICHAR> GridBytes;
			FStreamWriter<ANSICHAR> GridWriter(GridBytes, Ar.IsPersistent());
			GridPtr->writeMeta(GridWriter.GetStream());
			GridPtr->writeTopology(GridWriter.GetStream());
			GridPtr->writeTransform(GridWriter.GetStream());
			GridPtr->writeBuffers(GridWriter.GetStream());
			Ar << GridBytes;
		}
	}

	FORCEINLINE_DEBUGGABLE void SerializeGridData(FArchive& Ar, TMap<FIntVector, openvdb::GridBase::Ptr>& GridData)
	{
		int32 GridCount = -1;
		if (Ar.IsLoading())
		{
			Ar << GridCount;
			for (int32 i = 0; i < GridCount; ++i)
			{
				ReadGrid(Ar, GridData);
			}
		}
		else
		{
			//First get the non-null grid count
			for (auto i = GridData.CreateConstIterator(); i; ++i)
			{
				if (i.Value() != nullptr)
				{
					GridCount++;
				}
			}

			Ar << GridCount;
			for (auto i = GridData.CreateConstIterator(); i; ++i)
			{
				WriteGrid(Ar, i.Value());
			}
		}
	}

	FORCEINLINE_DEBUGGABLE void ReadMetadata(FArchive& Ar, openvdb::MetaMap& MetaMap)
	{
		check(Ar.IsLoading());

		TArray<ANSICHAR> MetaMapBytes;
		Ar << MetaMapBytes;

		FStreamReader<ANSICHAR> MetaMapFileStream(MetaMapBytes, Ar.IsPersistent());
		MetaMap.readMeta(MetaMapFileStream.GetStream());
	}

	FORCEINLINE_DEBUGGABLE void WriteMetadata(FArchive& Ar, const openvdb::MetaMap& MetaMap)
	{
		check(Ar.IsSaving());

		TArray<ANSICHAR> MetaMapBytes;
		FStreamWriter<ANSICHAR> MetaMapFileStream(MetaMapBytes, Ar.IsPersistent());
		MetaMap.writeMeta(MetaMapFileStream.GetStream());

		Ar << MetaMapBytes;
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
}
