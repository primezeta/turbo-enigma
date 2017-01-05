#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "UnrealNetwork.h"
#include "VoxelValueSources.h"
#include "VoxelDatabase.generated.h"

////TODO: See UWorldProxy which has checks for access from non-game threads. Might need to do the same for AVoxelDatabase
//struct FGridBaseValuesGeneratorArray;
//
//USTRUCT()
//struct FGridBaseValuesGeneratorItem : public FFastArraySerializerItem
//{
//	GENERATED_BODY()
//
//	FGridBaseValuesGeneratorItem()
//	{}
//
//	FGridBaseValuesGeneratorItem(const FText& GridText, AGridSource* Source)
//		: DisplayText(GridText), GridSource(Source)
//	{
//		GridId = GridSource->GridIndex;
//	}
//
//	friend FArchive& operator<<(FArchive& Ar, FGridBaseValuesGeneratorItem& Item)
//	{
//		Item.Serialize(Ar);
//		return Ar;
//	}
//
//	UPROPERTY()
//		FIntVector GridId;
//	UPROPERTY()
//		FText DisplayText;
//	UPROPERTY()
//		AGridSource* GridSource;
//
//	void Serialize(FArchive& Ar)
//	{
//		Ar << GridId;
//		Ar << DisplayText;
//		GridSource->Serialize(Ar);
//	}
//
//	void PreReplicatedRemove(const FGridBaseValuesGeneratorArray& InArraySerializer)
//	{
//	}
//
//	void PostReplicatedChange(const FGridBaseValuesGeneratorArray& InArraySerializer)
//	{
//	}
//
//	void PostReplicatedAdd(const FGridBaseValuesGeneratorArray& InArraySerializer)
//	{
//	}
//};
//
//USTRUCT()
//struct FGridBaseValuesGeneratorArray : public FFastArraySerializer
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//		TArray<FGridBaseValuesGeneratorItem> Items;
//
//	void Serialize(FArchive& Ar)
//	{
//		Items.BulkSerialize(Ar);
//		MarkArrayDirty();
//	}
//
//	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
//	{
//		return FFastArraySerializer::FastArrayDeltaSerialize<FGridBaseValuesGeneratorItem, FGridBaseValuesGeneratorArray>(Items, DeltaParms, *this);
//	}
//
//	const FGridBaseValuesGeneratorItem& AddItem(FGridBaseValuesGeneratorItem&& Item)
//	{
//		const int32 Index = Items.Add(Item);
//		MarkItemDirty(Items[Index]);
//		return Items[Index];
//	}
//
//	void RemoveItem(const FIntVector& GridId)
//	{
//		for (auto i = Items.CreateIterator(); i; ++i)
//		{
//			if (i->GridId == GridId)
//			{
//				Items.RemoveAt(i.GetIndex());
//				MarkArrayDirty();
//				return;
//			}
//		}
//	}
//};
//
//template<>
//struct TStructOpsTypeTraits<FGridBaseValuesGeneratorArray> : public TStructOpsTypeTraitsBase
//{
//	enum
//	{
//		WithNetDeltaSerializer = true,
//	};
//};

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
		//ServerDatabaseIsInitialized = false;
	}

	virtual void Serialize(FArchive& Ar) override;

	//UPROPERTY(ReplicatedUsing=OnRep_ServerDatabaseIsInitialized)
	//	bool ServerDatabaseIsInitialized;

	//UPROPERTY(ReplicatedUsing=OnRep_GridBaseValuesGeneratorArray)
	//	FGridBaseValuesGeneratorArray GridBaseValuesGenerators;

	//UFUNCTION()
	//	void ServerInitializeDatabase(const FString& Filename);

	//UFUNCTION()
	//	void OnRep_ServerDatabaseIsInitialized();

	//UFUNCTION()
	//	void OnRep_GridBaseValuesGeneratorArray();

	UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void AddGrid(TSubclassOf<AGridSource> GridSourceClass, const FText& GridText);
	bool AddGrid_Validate(TSubclassOf<AGridSource> GridSourceClass, const FText& GridText);
	void AddGrid_Implementation(TSubclassOf<AGridSource> GridSourceClass, const FText& GridText);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable, WithValidation)
	//	void ChangeVoxel(const FIntVector& GridId, const FIntVector& IndexCoord, const FVoxel& Voxel, bool IsActive);
	//bool ChangeVoxel_Validate(const FIntVector& GridId, const FIntVector& IndexCoord, const FVoxel& Voxel, bool IsActive);
	//void ChangeVoxel_Implementation(const FIntVector& GridId, const FIntVector& IndexCoord, const FVoxel& Voxel, bool IsActive);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformAffine(const FIntVector& GridId, const FAffineCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformAffine_Implementation(const FIntVector& GridId, const FAffineCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformUnitary(const FIntVector& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformUnitary_Implementation(const FIntVector& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformScale(const FIntVector& GridId, const FScaleCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformScale_Implementation(const FIntVector& GridId, const FScaleCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformUniformScale(const FIntVector& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformUniformScale_Implementation(const FIntVector& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformTranslation(const FIntVector& GridId, const FTranslationCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformTranslation_Implementation(const FIntVector& GridId, const FTranslationCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformScaleTranslation(const FIntVector& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformScaleTranslation_Implementation(const FIntVector& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformUniformScaleTranslation(const FIntVector& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformUniformScaleTranslation_Implementation(const FIntVector& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void SetCoordinateTransformNonlinearFrustum(const FIntVector& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform);
	//void SetCoordinateTransformNonlinearFrustum_Implementation(const FIntVector& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateRotation(const FIntVector& GridId, ETransformOp Op, float Radians, EAxis::Type Axis);
	//void InsertCoordinateRotation_Implementation(const FIntVector& GridId, ETransformOp Op, float Radians, EAxis::Type Axis);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateTranslation(const FIntVector& GridId, ETransformOp Op, const FVector &InTranslation);
	//void InsertCoordinateTranslation_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector &InTranslation);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateScale(const FIntVector& GridId, ETransformOp Op, const FVector &InScale);
	//void InsertCoordinateScale_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector &InScale);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateUniformScale(const FIntVector& GridId, ETransformOp Op, float Scale);
	//void InsertCoordinateUniformScale_Implementation(const FIntVector& GridId, ETransformOp Op, float Scale);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateShear(const FIntVector& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1);
	//void InsertCoordinateShear_Implementation(const FIntVector& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateMatrix4dMultiply(const FIntVector& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW);
	//void InsertCoordinateMatrix4dMultiply_Implementation(const FIntVector& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW);

	//UFUNCTION(Category = VoxelDatabase, BlueprintCallable, NetMulticast, Reliable)
	//	void InsertCoordinateMatrix3dMultiply(const FIntVector& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ);
	//void InsertCoordinateMatrix3dMultiply_Implementation(const FIntVector& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ);

private:
	//FString SourceFilename;
};
