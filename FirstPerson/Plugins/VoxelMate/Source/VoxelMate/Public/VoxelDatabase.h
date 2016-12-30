#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "UnrealNetwork.h"
#include "VoxelValueSources.h"
#include "VoxelDatabase.generated.h"

//TODO: See UWorldProxy which has checks for access from non-game threads. Might need to do the same for AVoxelDatabase
struct FGridBaseValuesGeneratorArray;

USTRUCT()
struct FGridBaseValuesGeneratorItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGridBaseValuesGeneratorItem()
	{}

	FGridBaseValuesGeneratorItem(const FText& GridText, AValueSource* Source)
		: DisplayText(GridText), ValueSource(Source)
	{
		GridId = FGuid::NewGuid();
	}

	friend FArchive& operator<<(FArchive& Ar, FGridBaseValuesGeneratorItem& Item)
	{
		Item.Serialize(Ar);
		return Ar;
	}

	UPROPERTY()
		FGuid GridId;
	UPROPERTY()
		FText DisplayText;
	UPROPERTY()
		AValueSource* ValueSource;

	void Serialize(FArchive& Ar)
	{
		Ar << GridId;
		Ar << DisplayText;
		ValueSource->Serialize(Ar);
	}

	void PreReplicatedRemove(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}

	void PostReplicatedChange(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}

	void PostReplicatedAdd(const FGridBaseValuesGeneratorArray& InArraySerializer)
	{
	}
};

USTRUCT()
struct FGridBaseValuesGeneratorArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FGridBaseValuesGeneratorItem> Items;

	void Serialize(FArchive& Ar)
	{
		Items.BulkSerialize(Ar);
		MarkArrayDirty();
	}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGridBaseValuesGeneratorItem, FGridBaseValuesGeneratorArray>(Items, DeltaParms, *this);
	}

	const FGridBaseValuesGeneratorItem& AddItem(FGridBaseValuesGeneratorItem&& Item)
	{
		const int32 Index = Items.Add(Item);
		MarkItemDirty(Items[Index]);
		return Items[Index];
	}

	void RemoveItem(const FGuid& GridId)
	{
		for (auto i = Items.CreateIterator(); i; ++i)
		{
			if (i->GridId == GridId)
			{
				Items.RemoveAt(i.GetIndex());
				MarkArrayDirty();
				return;
			}
		}
	}
};

template<>
struct TStructOpsTypeTraits<FGridBaseValuesGeneratorArray> : public TStructOpsTypeTraitsBase
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

//USTRUCT(ClassGroup = VoxelMate, NotBlueprintable, NotPlaceable)
//struct FVoxelChange
//{
//
//};

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class AVoxelDatabase : public AActor
{
    GENERATED_BODY()

public:
	AVoxelDatabase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SetReplicates(true);
		SetReplicateMovement(false);
		SetActorEnableCollision(false);
		bNetTemporary = 0;
		bAutoDestroyWhenFinished = 0;
		bCanBeDamaged = 0;
		bAlwaysRelevant = 1;
		bNetLoadOnClient = 1;
	}

	virtual void Serialize(FArchive& Ar) override;

	UPROPERTY(ReplicatedUsing=OnRep_GridBaseValuesGeneratorArray)
		FGridBaseValuesGeneratorArray GridBaseValuesGenerators;

	UFUNCTION()
		void OnRep_GridBaseValuesGeneratorArray();

	UFUNCTION(Category = VoxelDatabase, Server, Reliable, WithValidation)
		void AddGrid(AValueSource* ValueSource, const FText& GridText);
	bool AddGrid_Validate(AValueSource* ValueSource, const FText& GridText);
	void AddGrid_Implementation(AValueSource* ValueSource, const FText& GridText);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable, WithValidation)
		void ChangeVoxel(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive);	
	bool ChangeVoxel_Validate(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive);
	void ChangeVoxel_Implementation(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformAffine(const FGuid& GridId, const FAffineCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformAffine_Implementation(const FGuid& GridId, const FAffineCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformUnitary(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformUnitary_Implementation(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformScale(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformScale_Implementation(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformUniformScale(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformUniformScale_Implementation(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformTranslation(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformTranslation_Implementation(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformScaleTranslation(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformScaleTranslation_Implementation(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformUniformScaleTranslation(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformUniformScaleTranslation_Implementation(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void SetCoordinateTransformNonlinearFrustum(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform);
	void SetCoordinateTransformNonlinearFrustum_Implementation(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridRotation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis);
	void InsertGridRotation_Implementation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridTranslation(const FGuid& GridId, ETransformOp Op, const FVector &InTranslation);
	void InsertGridTranslation_Implementation(const FGuid& GridId, ETransformOp Op, const FVector &InTranslation);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridScale(const FGuid& GridId, ETransformOp Op, const FVector &InScale);
	void InsertGridScale_Implementation(const FGuid& GridId, ETransformOp Op, const FVector &InScale);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridUniformScale(const FGuid& GridId, ETransformOp Op, float Scale);
	void InsertGridUniformScale_Implementation(const FGuid& GridId, ETransformOp Op, float Scale);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridShear(const FGuid& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1);
	void InsertGridShear_Implementation(const FGuid& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridMatrix4dMultiply(const FGuid& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW);
	void InsertGridMatrix4dMultiply_Implementation(const FGuid& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW);

	UFUNCTION(Category = VoxelDatabase, NetMulticast, Reliable)
		void InsertGridMatrix3dMultiply(const FGuid& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ);
	void InsertGridMatrix3dMultiply_Implementation(const FGuid& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ);
};
