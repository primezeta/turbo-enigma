#include "VoxelMate.h"

#ifndef _VOXEL_MATE_
#define _VOXEL_MATE_
#include "VoxelDatabaseTypes.h"
#include <boost/iostreams/stream.hpp>
#pragma warning(push)
#pragma warning(once:4211 4503 4800 4146 4714)
#include <openvdb/openvdb.h>

#define VOXEL_TYPE_EXTERN(VoxelType)\
extern std::ostream& operator<<(std::ostream& os, const VoxelType& InValue);\
extern bool operator==(const VoxelType& InLhs, const VoxelType& InRhs);\
extern bool operator<(const VoxelType& InLhs, const VoxelType& InRhs);\
extern bool operator>(const VoxelType& InLhs, const VoxelType& InRhs);\
extern VoxelType operator+(const VoxelType& InLhs, const VoxelType& InRhs);\
extern VoxelType operator-(const VoxelType& InLhs, const VoxelType& InRhs);\
extern VoxelType operator+(const VoxelType& InLhs, const float &InRhs);\
extern VoxelType Abs(const VoxelType& InValue);\
extern VoxelType operator-(const VoxelType& InValue);

VOXEL_TYPE_EXTERN(FVoxel)

#define META_TYPE_EXTERN(MetaType)\
extern std::ostream& operator<<(std::ostream& os, const MetaType& InValue);\
extern bool operator==(const MetaType& InLhs, const MetaType& InRhs);

META_TYPE_EXTERN(FMetadataBool)
META_TYPE_EXTERN(FMetadataFloat)
META_TYPE_EXTERN(FMetadataInt32)
META_TYPE_EXTERN(FMetadataUInt8)
META_TYPE_EXTERN(FMetadataVector)
META_TYPE_EXTERN(FMetadataColor)
META_TYPE_EXTERN(FMetadataLinearColor)
META_TYPE_EXTERN(FMetadataIntVector)
META_TYPE_EXTERN(FMetadataRotator)
META_TYPE_EXTERN(FMetadataQuat)
META_TYPE_EXTERN(FMetadataBox)
META_TYPE_EXTERN(FMetadataDateTime)
META_TYPE_EXTERN(FMetadataString)
META_TYPE_EXTERN(FMetadataText)

//TODO
//template<> struct openvdb::math::Tolerance<>()
//template<> struct openvdb::math::Delta<>()

#pragma warning(pop)

#endif
