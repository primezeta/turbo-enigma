#pragma once
#include "Delegate.h"
#include "VoxelDatabaseTypes.h"
#include "VoxelValueSources.generated.h"

UCLASS(Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API AGridSource : public AActor
{
	GENERATED_BODY()

public:
	AGridSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		SetReplicates(true);
		IsFloatSavedAsHalf = false;
	}

	UFUNCTION(Category = VoxelGridSource, BlueprintCallable)
		FString ToString()
		{
			return FString::Printf(TEXT("%s%s"), *GridSectionDimensions.ToString(), IsFloatSavedAsHalf ? TEXT(";half") : TEXT(""));
		}

	virtual void Serialize(FArchive& Ar) override
	{
		Ar << GridSectionDimensions;
		Ar << IsFloatSavedAsHalf;
	}
	
	UPROPERTY(Replicated, BlueprintReadOnly)
		FIntVector GridSectionDimensions;
	UPROPERTY(Replicated, BlueprintReadOnly)
		bool IsFloatSavedAsHalf;

	UFUNCTION(Category = VoxelGridSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxel& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxel& OutValue) const
	{
		check(false);
	}
};

//template <typename ValueType>
//struct TGridSourceKeyFuncs :
//	BaseKeyFuncs<
//	TPair<AGridSource, ValueType>,
//	FGuid
//	>
//{
//private:
//	typedef BaseKeyFuncs<
//		TPair<AGridSource, ValueType>,
//		FGuid
//	> Super;
//
//public:
//	typedef typename Super::ElementInitType ElementInitType;
//	typedef typename Super::KeyInitType     KeyInitType;
//
//	static KeyInitType GetSetKey(ElementInitType Element)
//	{
//		return Element.Key.UniqueID;
//	}
//
//	static bool Matches(KeyInitType A, KeyInitType B)
//	{
//		return A == B;
//	}
//
//	static uint32 GetKeyHash(KeyInitType Key)
//	{
//		return FGuid::GetTypeHash(Key);
//	}
//};
