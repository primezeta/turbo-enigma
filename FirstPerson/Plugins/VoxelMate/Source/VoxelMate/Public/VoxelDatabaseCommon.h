#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.generated.h"

#ifdef UE_BUILD_DEBUG
    #define VOXELMATEINLINE
#else
    #define VOXELMATEINLINE FORCEINLINE
#endif

UENUM(BlueprintType)
enum class EGridCompression : uint8
{
    GridCompressionNone       UMETA(DisplayName = "None"),
    GridCompressionZip        UMETA(DisplayName = "Zip"),
    GridCompressionActiveMask UMETA(DisplayName = "Active Mask"),
    GridCompressionBlosc      UMETA(DisplayName = "Blosc"),
};

UENUM(BlueprintType)
enum class EGridClass : uint8
{
    GridClassUnknown   UMETA(DisplayName = "Unknown Grid Class"),
    GridClassLevelSet  UMETA(DisplayName = "Level Set"),
    GridClassFogVolume UMETA(DisplayName = "Fog Volume"),
    GridClassStaggered UMETA(DisplayName = "Staggered"),
};

UENUM(BlueprintType)
enum class EVectorTypeClass : uint8
{
    VectorTypeInvariant             UMETA(DisplayName = "Invariant"),
    VectorTypeCovariant             UMETA(DisplayName = "Covariant"),
    VectorTypeCovariantNormalize    UMETA(DisplayName = "Covariant Normalize"),
    VectorTypeContravariantRelative UMETA(DisplayName = "Contravariant Relative"),
    VectorTypeContravariantAbsolute UMETA(DisplayName = "Contravariant Absolute")
};

UENUM(BlueprintType)
enum class ETransformOp : uint8
{
    PreOp,
    PostOp
};

UENUM(BlueprintType)
enum class EVoxelIterator : uint8
{
    InactiveVoxelsIter UMETA(DisplayName = "Inactive Voxels Iterator"),
    ActiveVoxelsIter   UMETA(DisplayName = "Active Voxels Iterator"),
    AllVoxelsIter      UMETA(DisplayName = "All Voxels Iterator")
};

UENUM(BlueprintType)
enum class ECoordinateTransformType : uint8
{
	Affine                      UMETA(DisplayName = "Affine"),
	Unitary                     UMETA(DisplayName = "Unitary"),
	Scale                       UMETA(DisplayName = "Scale"),
	UniformScale                UMETA(DisplayName = "Uniform Scale"),
	Translation                 UMETA(DisplayName = "Translation"),
	ScaleTranslation            UMETA(DisplayName = "Scale and Translation"),
	UniformScaleTranslation     UMETA(DisplayName = "Uniform Scale and Translation"),
	NonlinearFrustum            UMETA(DisplayName = "Nonlinear Frustum"),
};

UENUM(BlueprintType)
enum class EVoxelType : uint8
{
	Bool                        UMETA(DisplayName = "bool"),
	UInt8                       UMETA(DisplayName = "uint8"),
	Int32                       UMETA(DisplayName = "int32"),
	Float                       UMETA(DisplayName = "float"),
	IntVector                   UMETA(DisplayName = "FIntVector"),
	Vector                      UMETA(DisplayName = "FVector"),
};

UENUM(BlueprintType)
enum class EVoxelDatabaseMetadataType : uint8
{
    Bool                                        UMETA(DisplayName = "bool"),
    Float                                       UMETA(DisplayName = "float"),
	Int32                                       UMETA(DisplayName = "int32"),
	UInt8                                       UMETA(DisplayName = "uint8"),
    Vector                                      UMETA(DisplayName = "FVector"),
    Color                                       UMETA(DisplayName = "FColor"),
    LinearColor                                 UMETA(DisplayName = "FLinearColor"),
    IntVector                                   UMETA(DisplayName = "FIntVector"),
    Rotator                                     UMETA(DisplayName = "FRotator"),
    Quat                                        UMETA(DisplayName = "FQuat"),
    Box                                         UMETA(DisplayName = "FBox"),
    DateTime                                    UMETA(DisplayName = "FDateTime"),
    String                                      UMETA(DisplayName = "FString"),
};
