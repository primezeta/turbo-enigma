#pragma once
#include "EngineMinimal.h"

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
enum class ENoiseType : uint8
{
    Value    UMETA(DisplayName = "Value Noise"),
    Perlin   UMETA(DisplayName = "Perlin Noise"),
    Simplex  UMETA(DisplayName = "Simplex Noise"),
    White    UMETA(DisplayName = "White Noise"),
    Cellular UMETA(DisplayName = "Cellular Noise")
};

UENUM(BlueprintType)
enum class ENoiseFractalMode : uint8
{
    None                     UMETA(DisplayName = "None"),
    FractionalBrownianMotion UMETA(DisplayName = "Fractional Brownian Motion Fractal"),
    Billow                   UMETA(DisplayName = "Billow Fractal"),
    RigidMulti               UMETA(DisplayName = "Rigid Multi Fractal")
};

UENUM(BlueprintType)
enum class ENoiseCellularDistanceMode : uint8
{
    Euclidean UMETA(DisplayName = "Euclidean"),
    Manhattan UMETA(DisplayName = "Manhattan"),
    Natural   UMETA(DisplayName = "Natural")
};

UENUM(BlueprintType)
enum class ENoiseCellularReturnType : uint8
{
    CellValue    UMETA(DisplayName = "Cell Value Return"),
    Distance     UMETA(DisplayName = "Distance Return"),
    Distance2    UMETA(DisplayName = "Distance2 Return"),
    Distance2Add UMETA(DisplayName = "Distance2Add Return"),
    Distance2Sub UMETA(DisplayName = "Distance2Sub Return"),
    Distance2Mul UMETA(DisplayName = "Distance2Mul Return"),
    Distance2Div UMETA(DisplayName = "Distance2Div Return")
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
enum class EExtractGridSurfaceOp : uint8
{
    HeightMap UMETA(DisplayName = "Height Map Surface Extraction"),
    Density   UMETA(DisplayName = "Density Grid Surface Extraction")
};

UENUM(BlueprintType)
enum class EVoxelScalarType : uint8
{
    Bool                        UMETA(DisplayName = "bool"),
    UInt8                       UMETA(DisplayName = "uint8"),
    Int32                       UMETA(DisplayName = "int32"),
};

UENUM(BlueprintType)
enum class EVoxelFloatScalarType : uint8
{
    Float                       UMETA(DisplayName = "float"),
};

UENUM(BlueprintType)
enum class EVoxelVectorType : uint8
{
    IntVector                   UMETA(DisplayName = "FIntVector"),
};

UENUM(BlueprintType)
enum class EVoxelFloatVectorType : uint8
{
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
    AffineCoordinateTransform                   UMETA(DisplayName = "FAffineCoordinateTransform"),
    UnitaryCoordinateTransform                  UMETA(DisplayName = "FUnitaryCoordinateTransform"),
    ScaleCoordinateTransform                    UMETA(DisplayName = "FScaleCoordinateTransform"),
    UniformScaleCoordinateTransform             UMETA(DisplayName = "FUniformScaleCoordinateTransform"),
    TranslationCoordinateTransform              UMETA(DisplayName = "FTranslationCoordinateTransform"),
    ScaleTranslationCoordinateTransform         UMETA(DisplayName = "FScaleTranslationCoordinateTransform"),
    UniformScaleTranslationCoordinateTransform  UMETA(DisplayName = "FUniformScaleTranslationCoordinateTransform"),
    NonlinearFrustumCoordinateTransform         UMETA(DisplayName = "FNonlinearFrustumCoordinateTransform"),
};
