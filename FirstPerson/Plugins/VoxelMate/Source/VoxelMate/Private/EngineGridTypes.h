#pragma once
#include "Platform.h"
#include "EngineMinimal.h"
#include "VoxelDatabaseTypes.h"
#include "GridCoordinateTransforms.h"
#include <openvdb/openvdb.h>

#include "VoxelDatabaseTypes.h"
#include <boost/iostreams/stream.hpp>

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

VOXEL_TYPE_EXTERN(FVoxelBool)
VOXEL_TYPE_EXTERN(FVoxelUInt8)
VOXEL_TYPE_EXTERN(FVoxelInt32)
VOXEL_TYPE_EXTERN(FVoxelFloat)
VOXEL_TYPE_EXTERN(FVoxelVector)
VOXEL_TYPE_EXTERN(FVoxelIntVector)

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
META_TYPE_EXTERN(FMetadataAffineCoordinateTransform)
META_TYPE_EXTERN(FMetadataUnitaryCoordinateTransform)
META_TYPE_EXTERN(FMetadataScaleCoordinateTransform)
META_TYPE_EXTERN(FMetadataUniformScaleCoordinateTransform)
META_TYPE_EXTERN(FMetadataTranslationCoordinateTransform)
META_TYPE_EXTERN(FMetadataScaleTranslationCoordinateTransform)
META_TYPE_EXTERN(FMetadataUniformScaleTranslationCoordinateTransform)
META_TYPE_EXTERN(FMetadataNonlinearFrustumCoordinateTransform)

template<>
VOXELMATEINLINE FVoxelBool openvdb::zeroVal<FVoxelBool>()
{
	return FVoxelBool::ZeroValue;
}

template<>
VOXELMATEINLINE FVoxelUInt8 openvdb::zeroVal<FVoxelUInt8>()
{
	return FVoxelUInt8::ZeroValue;
}

template<>
VOXELMATEINLINE FVoxelInt32 openvdb::zeroVal<FVoxelInt32>()
{
	return FVoxelInt32::ZeroValue;
}

template<>
VOXELMATEINLINE FVoxelFloat openvdb::zeroVal<FVoxelFloat>()
{
	return FVoxelFloat::ZeroValue;
}

template<>
VOXELMATEINLINE FVoxelVector openvdb::zeroVal<FVoxelVector>()
{
	return FVoxelVector::ZeroValue;
}

template<>
VOXELMATEINLINE FVoxelIntVector openvdb::zeroVal<FVoxelIntVector>()
{
	return FVoxelIntVector::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataBool openvdb::zeroVal<FMetadataBool>()
{
	return FMetadataBool::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataFloat openvdb::zeroVal<FMetadataFloat>()
{
	return FMetadataFloat::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataInt32 openvdb::zeroVal<FMetadataInt32>()
{
	return FMetadataInt32::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataUInt8 openvdb::zeroVal<FMetadataUInt8>()
{
	return FMetadataUInt8::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataVector openvdb::zeroVal<FMetadataVector>()
{
	return FMetadataVector::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataColor openvdb::zeroVal<FMetadataColor>()
{
	return FMetadataColor::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataLinearColor openvdb::zeroVal<FMetadataLinearColor>()
{
	return FMetadataLinearColor::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataIntVector openvdb::zeroVal<FMetadataIntVector>()
{
	return FMetadataIntVector::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataRotator openvdb::zeroVal<FMetadataRotator>()
{
	return FMetadataRotator::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataQuat openvdb::zeroVal<FMetadataQuat>()
{
	return FMetadataQuat::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataBox openvdb::zeroVal<FMetadataBox>()
{
	return FMetadataBox::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataDateTime openvdb::zeroVal<FMetadataDateTime>()
{
	return FMetadataDateTime::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataString openvdb::zeroVal<FMetadataString>()
{
	return FMetadataString::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataText openvdb::zeroVal<FMetadataText>()
{
	return FMetadataText::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataAffineCoordinateTransform openvdb::zeroVal<FMetadataAffineCoordinateTransform>()
{
	return FMetadataAffineCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataUnitaryCoordinateTransform openvdb::zeroVal<FMetadataUnitaryCoordinateTransform>()
{
	return FMetadataUnitaryCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataScaleCoordinateTransform openvdb::zeroVal<FMetadataScaleCoordinateTransform>()
{
	return FMetadataScaleCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataUniformScaleCoordinateTransform openvdb::zeroVal<FMetadataUniformScaleCoordinateTransform>()
{
	return FMetadataUniformScaleCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataTranslationCoordinateTransform openvdb::zeroVal<FMetadataTranslationCoordinateTransform>()
{
	return FMetadataTranslationCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataScaleTranslationCoordinateTransform openvdb::zeroVal<FMetadataScaleTranslationCoordinateTransform>()
{
	return FMetadataScaleTranslationCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataUniformScaleTranslationCoordinateTransform openvdb::zeroVal<FMetadataUniformScaleTranslationCoordinateTransform>()
{
	return FMetadataUniformScaleTranslationCoordinateTransform::ZeroValue;
}

template<>
VOXELMATEINLINE FMetadataNonlinearFrustumCoordinateTransform openvdb::zeroVal<FMetadataNonlinearFrustumCoordinateTransform>()
{
	return FMetadataNonlinearFrustumCoordinateTransform::ZeroValue;
}

//TODO
//template<> struct openvdb::math::Tolerance<>()
//template<> struct openvdb::math::Delta<>()
