#include "VoxelMatePCH.h"
#include "UnrealNetwork.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabase.h"

//TODO idea for faster grid lookup
//TArray<FGridFactory::ValueTypeWeakPtr> CachedGrids; //Careful of anyone accessing this from other threads

void AVoxelGridProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelGridProxy, GridId);
}

//#define GRID_PROXY_INTERFACE_IMPLEMENTATION(Type, TypeName)\
//AVoxelProxy* AVoxelGridProxy##TypeName::GetVoxel(const FIntVector& IndexCoord)\
//{\
//	AVoxelProxy** FindVoxelProxy = ActiveVoxels.FindByPredicate([&](AVoxelProxy*& VoxelProxy)\
//	{\
//		return VoxelProxy && VoxelProxy->IndexCoord == IndexCoord;\
//	});\
//	if (FindVoxelProxy)\
//	{\
//		return *FindVoxelProxy;\
//	}\
//	AVoxelProxy##TypeName* VoxelProxy = nullptr;\
//	if (VoxelDatabase)\
//	{\
//		VoxelProxy = NewObject<AVoxelProxy##TypeName>(this);\
//		check(VoxelProxy);\
//		VoxelProxy->IndexCoord = IndexCoord;\
//		VoxelProxy->VoxelValue = VoxelDatabase->GetVoxelValue<FVoxel##TypeName>(GridId, IndexCoord, VoxelProxy->IsVoxelActive).Value;\
//		ActiveVoxels.Add(VoxelProxy);\
//	}\
//	return VoxelProxy;\
//}\
//void AVoxelGridProxy##TypeName::FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const Type& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill)\
//{\
//	if (VoxelDatabase)\
//	{\
//		VoxelDatabase->FillGrid<FVoxel##TypeName>(GridId, StartIndexCoord, FillDimensions, FVoxel##TypeName(Value), AreValuesActive, VoxelizeActiveTilesAfterFill);\
//	}\
//}\
//void AVoxelGridProxy##TypeName::SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxel##TypeName##Source>& ValueSourceInterface)\
//{\
//	UVoxel##TypeName##Source* ValueSource = Cast<UVoxel##TypeName##Source>(ValueSourceInterface.GetObject());\
//	if (VoxelDatabase && ValueSource)\
//	{\
//		if (VoxelIter == EVoxelIterator::InactiveVoxelsIter)\
//		{\
//			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::InactiveVoxelsIter>(GridId, *ValueSource);\
//		}\
//		else if (VoxelIter == EVoxelIterator::ActiveVoxelsIter)\
//		{\
//			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::ActiveVoxelsIter>(GridId, *ValueSource);\
//		}\
//		else\
//		{\
//			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::AllVoxelsIter>(GridId, *ValueSource);\
//		}\
//	}\
//}\
//void AVoxelGridProxy##TypeName::ExtractSurface(EVoxelIterator VoxelIter)\
//{\
//	if(VoxelDatabase)\
//	{\
//		if (VoxelIter == EVoxelIterator::InactiveVoxelsIter)\
//		{\
//			VoxelDatabase->ExtractGridSurface<FVoxel##TypeName, EVoxelIterator::InactiveVoxelsIter>(GridId);\
//		}\
//		else if (VoxelIter == EVoxelIterator::ActiveVoxelsIter)\
//		{\
//			VoxelDatabase->ExtractGridSurface<FVoxel##TypeName, EVoxelIterator::ActiveVoxelsIter>(GridId);\
//		}\
//	}\
//}
//
//GRID_PROXY_INTERFACE_IMPLEMENTATION(bool, Bool)
//GRID_PROXY_INTERFACE_IMPLEMENTATION(float, Float)
//GRID_PROXY_INTERFACE_IMPLEMENTATION(int32, Int32)
//GRID_PROXY_INTERFACE_IMPLEMENTATION(uint8, UInt8)
//GRID_PROXY_INTERFACE_IMPLEMENTATION(FVector, Vector)
//GRID_PROXY_INTERFACE_IMPLEMENTATION(FIntVector, IntVector)
