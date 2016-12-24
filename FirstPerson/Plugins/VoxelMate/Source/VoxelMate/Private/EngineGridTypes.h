#pragma once
#include "VoxelDatabaseTypes.h"
#include <openvdb/openvdb.h>

template<>
FORCEINLINE FVoxelBool openvdb::zeroVal<FVoxelBool>()
{
	return FVoxelBool::ZeroValue;
}

template<>
FORCEINLINE FVoxelUInt8 openvdb::zeroVal<FVoxelUInt8>()
{
	return FVoxelUInt8::ZeroValue;
}

template<>
FORCEINLINE FVoxelInt32 openvdb::zeroVal<FVoxelInt32>()
{
	return FVoxelInt32::ZeroValue;
}

template<>
FORCEINLINE FVoxelFloat openvdb::zeroVal<FVoxelFloat>()
{
	return FVoxelFloat::ZeroValue;
}

template<>
FORCEINLINE FVoxelVector openvdb::zeroVal<FVoxelVector>()
{
	return FVoxelVector::ZeroValue;
}

template<>
FORCEINLINE FVoxelIntVector openvdb::zeroVal<FVoxelIntVector>()
{
	return FVoxelIntVector::ZeroValue;
}

template<>
FORCEINLINE FMetadataBool openvdb::zeroVal<FMetadataBool>()
{
	return FMetadataBool::ZeroValue;
}

template<>
FORCEINLINE FMetadataFloat openvdb::zeroVal<FMetadataFloat>()
{
	return FMetadataFloat::ZeroValue;
}

template<>
FORCEINLINE FMetadataInt32 openvdb::zeroVal<FMetadataInt32>()
{
	return FMetadataInt32::ZeroValue;
}

template<>
FORCEINLINE FMetadataUInt8 openvdb::zeroVal<FMetadataUInt8>()
{
	return FMetadataUInt8::ZeroValue;
}

template<>
FORCEINLINE FMetadataVector openvdb::zeroVal<FMetadataVector>()
{
	return FMetadataVector::ZeroValue;
}

template<>
FORCEINLINE FMetadataColor openvdb::zeroVal<FMetadataColor>()
{
	return FMetadataColor::ZeroValue;
}

template<>
FORCEINLINE FMetadataLinearColor openvdb::zeroVal<FMetadataLinearColor>()
{
	return FMetadataLinearColor::ZeroValue;
}

template<>
FORCEINLINE FMetadataIntVector openvdb::zeroVal<FMetadataIntVector>()
{
	return FMetadataIntVector::ZeroValue;
}

template<>
FORCEINLINE FMetadataRotator openvdb::zeroVal<FMetadataRotator>()
{
	return FMetadataRotator::ZeroValue;
}

template<>
FORCEINLINE FMetadataQuat openvdb::zeroVal<FMetadataQuat>()
{
	return FMetadataQuat::ZeroValue;
}

template<>
FORCEINLINE FMetadataBox openvdb::zeroVal<FMetadataBox>()
{
	return FMetadataBox::ZeroValue;
}

template<>
FORCEINLINE FMetadataDateTime openvdb::zeroVal<FMetadataDateTime>()
{
	return FMetadataDateTime::ZeroValue;
}

template<>
FORCEINLINE FMetadataString openvdb::zeroVal<FMetadataString>()
{
	return FMetadataString::ZeroValue;
}

template<>
FORCEINLINE FMetadataText openvdb::zeroVal<FMetadataText>()
{
	return FMetadataText::ZeroValue;
}
