#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.generated.h"

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

#ifdef WITH_ENGINE
#include "Engine.h"
#define GUnreal GEngine
#elif defined WITH_EDITOR
#include "Editor.h"
#define GUnreal GEditor
#endif

//From https://wiki.unrealengine.com/Log_Macro_with_Netmode_and_Colour
#define NETMODE_WORLD (((GUnreal == nullptr) || (GetWorld() == nullptr)) ? TEXT("") \
        : (GUnreal->GetNetMode(GetWorld()) == NM_Client) ? TEXT("[Client] ") \
        : (GUnreal->GetNetMode(GetWorld()) == NM_ListenServer) ? TEXT("[ListenServer] ") \
        : (GUnreal->GetNetMode(GetWorld()) == NM_DedicatedServer) ? TEXT("[DedicatedServer] ") \
        : TEXT("[Standalone] "))

#if _MSC_VER
#define FUNC_NAME TEXT(__FUNCTION__)
#else // FIXME - GCC?
#define FUNC_NAME TEXT(__func__)
#endif

#define NET_SCREEN_DEBUG(Object, Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (Msg == "") \
    { \
        TCHAR StdMsg[MAX_SPRINTF] = TEXT(""); \
        FCString::Sprintf(StdMsg, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *GetNameSafe(Object)); \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, StdMsg); \
    } \
    else \
    { \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("%s"), NETMODE_WORLD); \
    } \
}

#define NET_LOG(Object, LogCategory, Format, ...) \
{ \
    SET_WARN_COLOR(COLOR_CYAN);\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (Msg == "") \
    { \
        UE_LOG(LogCategory, Log, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *GetNameSafe(Object));\
    } \
    else \
    { \
        UE_LOG(LogCategory, Log, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *Msg);\
    } \
    CLEAR_WARN_COLOR();\
}

#define NET_LOG_STATIC(LogCategory, Format, ...) \
{ \
    SET_WARN_COLOR(COLOR_CYAN);\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogCategory, Log, TEXT("%s() : %s"), FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

#define NET_LOG_WARN_STATIC(LogCategory, Format, ...) \
{ \
    SET_WARN_COLOR( COLOR_YELLOW );\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogCategory, Log, TEXT("**WARNING** %s() : %s"), FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

#define NET_LOG_ERROR_STATIC(LogCategory, Format, ...) \
{ \
    SET_WARN_COLOR( COLOR_RED );\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogCategory, Log, TEXT("**ERROR** %s() : %s"), FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}
