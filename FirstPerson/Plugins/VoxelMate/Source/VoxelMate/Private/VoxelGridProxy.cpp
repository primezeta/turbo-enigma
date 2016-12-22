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

void AVoxelGridProxy::ResetCoordinateTransformToAffine(const FAffineCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUnitary(const FUnitaryCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToScale(const FScaleCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScale(const FUniformScaleCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToTranslation(const FTranslationCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToScaleTranslation(const FScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScaleTranslation(const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToNonlinearFrustum(const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridRotation(GridId, ETransformOp::PreOp, AngleRadians, Axis);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformTranslation(const FVector& InTranslation)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridTranslation(GridId, ETransformOp::PreOp, InTranslation);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformScale(const FVector& InScale)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridScale(GridId, ETransformOp::PreOp, InScale);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformUniformScale(float Scale)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridUniformScale(GridId, ETransformOp::PreOp, Scale);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridShear(GridId, ETransformOp::PreOp, Shear, FirstAxis, SecondAxis);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridMatrix4dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridMatrix3dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridRotation(GridId, ETransformOp::PostOp, AngleRadians, Axis);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformTranslation(const FVector& InTranslation)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridTranslation(GridId, ETransformOp::PostOp, InTranslation);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformScale(const FVector& InScale)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridScale(GridId, ETransformOp::PostOp, InScale);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformUniformScale(float Scale)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridUniformScale(GridId, ETransformOp::PostOp, Scale);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridShear(GridId, ETransformOp::PostOp, Shear, FirstAxis, SecondAxis);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridMatrix4dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
	if (VoxelDatabase)
	{
		VoxelDatabase->GridMatrix3dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow);
	}
}

#define GRID_PROXY_INTERFACE_IMPLEMENTATION(Type, TypeName)\
AVoxelProxy* AVoxelGridProxy##TypeName::GetVoxel(const FIntVector& IndexCoord)\
{\
	AVoxelProxy** FindVoxelProxy = ActiveVoxels.FindByPredicate([&](AVoxelProxy*& VoxelProxy)\
	{\
		return VoxelProxy && VoxelProxy->IndexCoord == IndexCoord;\
	});\
	if (FindVoxelProxy)\
	{\
		return *FindVoxelProxy;\
	}\
	AVoxelProxy##TypeName* VoxelProxy = nullptr;\
	if (VoxelDatabase)\
	{\
		VoxelProxy = NewObject<AVoxelProxy##TypeName>(this);\
		check(VoxelProxy);\
		VoxelProxy->IndexCoord = IndexCoord;\
		VoxelProxy->VoxelValue = VoxelDatabase->GetVoxelValue<FVoxel##TypeName>(GridId, IndexCoord, VoxelProxy->IsVoxelActive).Value;\
		ActiveVoxels.Add(VoxelProxy);\
	}\
	return VoxelProxy;\
}\
void AVoxelGridProxy##TypeName::FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const Type& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill)\
{\
	if (VoxelDatabase)\
	{\
		VoxelDatabase->FillGrid<FVoxel##TypeName>(GridId, StartIndexCoord, FillDimensions, FVoxel##TypeName(Value), AreValuesActive, VoxelizeActiveTilesAfterFill);\
	}\
}\
void AVoxelGridProxy##TypeName::SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxel##TypeName##Source>& ValueSourceInterface)\
{\
	UVoxel##TypeName##Source* ValueSource = Cast<UVoxel##TypeName##Source>(ValueSourceInterface.GetObject());\
	if (VoxelDatabase && ValueSource)\
	{\
		if (VoxelIter == EVoxelIterator::InactiveVoxelsIter)\
		{\
			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::InactiveVoxelsIter>(GridId, *ValueSource);\
		}\
		else if (VoxelIter == EVoxelIterator::ActiveVoxelsIter)\
		{\
			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::ActiveVoxelsIter>(GridId, *ValueSource);\
		}\
		else\
		{\
			VoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::AllVoxelsIter>(GridId, *ValueSource);\
		}\
	}\
}

GRID_PROXY_INTERFACE_IMPLEMENTATION(bool, Bool)
GRID_PROXY_INTERFACE_IMPLEMENTATION(float, Float)
GRID_PROXY_INTERFACE_IMPLEMENTATION(int32, Int32)
GRID_PROXY_INTERFACE_IMPLEMENTATION(uint8, UInt8)
GRID_PROXY_INTERFACE_IMPLEMENTATION(FVector, Vector)
GRID_PROXY_INTERFACE_IMPLEMENTATION(FIntVector, IntVector)
