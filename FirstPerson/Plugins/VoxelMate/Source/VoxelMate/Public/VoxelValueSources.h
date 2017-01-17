#pragma once
#include "Delegate.h"
#include "VoxelDatabaseTypes.h"
#include "VoxelValueSources.generated.h"

UCLASS(Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API UGridSource : public UObject
{
	GENERATED_BODY()

public:
	UGridSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		GridId = FGuid::NewGuid();
		NumActiveAdjacentRegions = FIntVector(0, 0, 0);
		RegionVoxelCount = FIntVector(1, 1, 1);
	}

	UPROPERTY(Replicated, BlueprintReadOnly)
		FGuid GridId;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		FIntVector NumActiveAdjacentRegions;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		FIntVector RegionVoxelCount;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

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
