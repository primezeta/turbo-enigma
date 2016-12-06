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
enum class EVoxelScalarType : uint8
{
    Bool                        UMETA(DisplayName = "bool"),
    Uint8                       UMETA(DisplayName = "uint8"),
    Uint16                      UMETA(DisplayName = "uint16"),
    Uint32                      UMETA(DisplayName = "uint32"),
    Uint64                      UMETA(DisplayName = "uint64"),
    Int8                        UMETA(DisplayName = "int8"),
    Int16                       UMETA(DisplayName = "int16"),
    Int32                       UMETA(DisplayName = "int32"),
    Int64                       UMETA(DisplayName = "int64"),
    Color                       UMETA(DisplayName = "FColor"),
};

UENUM(BlueprintType)
enum class EVoxelFloatScalarType : uint8
{
    Float                       UMETA(DisplayName = "float"),
    Double                      UMETA(DisplayName = "double"),
};

UENUM(BlueprintType)
enum class EVoxelVectorType : uint8
{
    PackedNormal                UMETA(DisplayName = "FPackedNormal"),
    PackedRGB10A2N              UMETA(DisplayName = "FPackedRGB10A2N"),
    PackedRGBA16N               UMETA(DisplayName = "FPackedRGBA16N"),
    IntPoint                    UMETA(DisplayName = "FIntPoint"),
    IntVector                   UMETA(DisplayName = "FIntVector"),
};

UENUM(BlueprintType)
enum class EVoxelFloatVectorType : uint8
{
    Vector                      UMETA(DisplayName = "FVector"),
    Vector4                     UMETA(DisplayName = "FVector4"),
    Vector2D                    UMETA(DisplayName = "FVector2D"),
    LinearColor                 UMETA(DisplayName = "FLinearColor"),
};

UENUM(BlueprintType)
enum class EVoxelDatabaseMetadataType : uint8
{
    Bool                                        UMETA(DisplayName = "bool"),
    Float                                       UMETA(DisplayName = "float"),
    Double                                      UMETA(DisplayName = "double"),
    Uint8                                       UMETA(DisplayName = "uint8"),
    Uint16                                      UMETA(DisplayName = "uint16"),
    Uint32                                      UMETA(DisplayName = "uint32"),
    Uint64                                      UMETA(DisplayName = "uint64"),
    Int8                                        UMETA(DisplayName = "int8"),
    Int16                                       UMETA(DisplayName = "int16"),
    Int32                                       UMETA(DisplayName = "int32"),
    Int64                                       UMETA(DisplayName = "int64"),
    Vector                                      UMETA(DisplayName = "FVector"),
    Vector4                                     UMETA(DisplayName = "FVector4"),
    Vector2D                                    UMETA(DisplayName = "FVector2D"),
    Color                                       UMETA(DisplayName = "FColor"),
    LinearColor                                 UMETA(DisplayName = "FLinearColor"),
    PackedNormal                                UMETA(DisplayName = "FPackedNormal"),
    PackedRGB10A2N                              UMETA(DisplayName = "FPackedRGB10A2N"),
    PackedRGBA16N                               UMETA(DisplayName = "FPackedRGBA16N"),
    IntPoint                                    UMETA(DisplayName = "FIntPoint"),
    IntVector                                   UMETA(DisplayName = "FIntVector"),
    FloatRangeBound                             UMETA(DisplayName = "FFloatRangeBound"),
    FloatRange                                  UMETA(DisplayName = "FFloatRange"),
    Int32RangeBound                             UMETA(DisplayName = "FInt32RangeBound"),
    Int32Range                                  UMETA(DisplayName = "FInt32Range"),
    FloatInterval                               UMETA(DisplayName = "FFloatInterval"),
    Int32Interval                               UMETA(DisplayName = "FInt32Interval"),
    TwoVectors                                  UMETA(DisplayName = "FTwoVectors"),
    Plane                                       UMETA(DisplayName = "FPlane"),
    Rotator                                     UMETA(DisplayName = "FRotator"),
    Quat                                        UMETA(DisplayName = "FQuat"),
    Box                                         UMETA(DisplayName = "FBox"),
    Box2D                                       UMETA(DisplayName = "FBox2D"),
    BoxSphereBounds                             UMETA(DisplayName = "FBoxSphereBounds"),
    OrientedBox                                 UMETA(DisplayName = "FOrientedBox"),
    Matrix                                      UMETA(DisplayName = "FMatrix"),
    InterpCurvePointFloat                       UMETA(DisplayName = "FInterpCurvePointFloat"),
    InterpCurveFloat                            UMETA(DisplayName = "FInterpCurveFloat"),
    InterpCurvePointVector2D                    UMETA(DisplayName = "FInterpCurvePointVector2D"),
    InterpCurveVector2D                         UMETA(DisplayName = "FInterpCurveVector2D"),
    InterpCurvePointVector                      UMETA(DisplayName = "FInterpCurvePointVector"),
    InterpCurveVector                           UMETA(DisplayName = "FInterpCurveVector"),
    InterpCurvePointQuat                        UMETA(DisplayName = "FInterpCurvePointQuat"),
    InterpCurveQuat                             UMETA(DisplayName = "FInterpCurveQuat"),
    InterpCurvePointTwoVectors                  UMETA(DisplayName = "FInterpCurvePointTwoVectors"),
    InterpCurveTwoVectors                       UMETA(DisplayName = "FInterpCurveTwoVectors"),
    InterpCurvePointLinearColor                 UMETA(DisplayName = "FInterpCurvePointLinearColor"),
    InterpCurveLinearColor                      UMETA(DisplayName = "FInterpCurveLinearColor"),
    Transform                                   UMETA(DisplayName = "FTransform"),
    RandomStream                                UMETA(DisplayName = "FRandomStream"),
    DateTime                                    UMETA(DisplayName = "FDateTime"),
    Timespan                                    UMETA(DisplayName = "FTimespan"),
    StringAssetReference                        UMETA(DisplayName = "FStringAssetReference"),
    StringClassReference                        UMETA(DisplayName = "FStringClassReference"),
    String                                      UMETA(DisplayName = "FString"),
    Name                                        UMETA(DisplayName = "FName"),
    Text                                        UMETA(DisplayName = "FText"),
    AffineCoordinateTransform                   UMETA(DisplayName = "FAffineCoordinateTransform"),
    UnitaryCoordinateTransform                  UMETA(DisplayName = "FUnitaryCoordinateTransform"),
    ScaleCoordinateTransform                    UMETA(DisplayName = "FScaleCoordinateTransform"),
    UniformScaleCoordinateTransform             UMETA(DisplayName = "FUniformScaleCoordinateTransform"),
    TranslationCoordinateTransform              UMETA(DisplayName = "FTranslationCoordinateTransform"),
    ScaleTranslationCoordinateTransform         UMETA(DisplayName = "FScaleTranslationCoordinateTransform"),
    UniformScaleTranslationCoordinateTransform  UMETA(DisplayName = "FUniformScaleTranslationCoordinateTransform"),
    NonlinearFrustumCoordinateTransform         UMETA(DisplayName = "FNonlinearFrustumCoordinateTransform"),
};
