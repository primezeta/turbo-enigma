#include "VoxelMatePCH.h"
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
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUnitary(const FUnitaryCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToScale(const FScaleCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScale(const FUniformScaleCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToTranslation(const FTranslationCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToScaleTranslation(const FScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScaleTranslation(const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::ResetCoordinateTransformToNonlinearFrustum(const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->SetCoordinateTransform(GridId, InCoordinateTransform);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridRotation(GridId, ETransformOp::PreOp, AngleRadians, Axis);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformTranslation(const FVector& InTranslation)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridTranslation(GridId, ETransformOp::PreOp, InTranslation);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformScale(const FVector& InScale)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridScale(GridId, ETransformOp::PreOp, InScale);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformUniformScale(float Scale)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridUniformScale(GridId, ETransformOp::PreOp, Scale);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridShear(GridId, ETransformOp::PreOp, Shear, FirstAxis, SecondAxis);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridMatrix4dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
	}
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridMatrix3dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridRotation(GridId, ETransformOp::PostOp, AngleRadians, Axis);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformTranslation(const FVector& InTranslation)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridTranslation(GridId, ETransformOp::PostOp, InTranslation);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformScale(const FVector& InScale)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridScale(GridId, ETransformOp::PostOp, InScale);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformUniformScale(float Scale)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridUniformScale(GridId, ETransformOp::PostOp, Scale);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridShear(GridId, ETransformOp::PostOp, Shear, FirstAxis, SecondAxis);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridMatrix4dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
	}
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
	if (AVoxelDatabase::VoxelMateVoxelDatabase)
	{
		AVoxelDatabase::VoxelMateVoxelDatabase->GridMatrix3dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow);
	}
}

#define GRID_PROXY_INTERFACE_IMPLEMENTATION(TypeName)\
AVoxelProxy* AVoxelGridProxy##TypeName::GetVoxel_Implementation(const FIntVector& IndexCoord)\
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
	if (AVoxelDatabase::VoxelMateVoxelDatabase)\
	{\
		VoxelProxy = NewObject<AVoxelProxy##TypeName>(this);\
		check(VoxelProxy);\
		VoxelProxy->IndexCoord = IndexCoord;\
		VoxelProxy->VoxelValue = AVoxelDatabase::VoxelMateVoxelDatabase->GetVoxelValue<FVoxel##TypeName>(GridId, IndexCoord, VoxelProxy->IsVoxelActive).Value;\
		ActiveVoxels.Add(VoxelProxy);\
	}\
	return VoxelProxy;\
}\
void AVoxelGridProxy##TypeName::FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill)\
{\
	const AVoxelProxy##TypeName* VoxelProxy##TypeName = Cast<AVoxelProxy##TypeName>(VoxelProxy);\
	if (AVoxelDatabase::VoxelMateVoxelDatabase && VoxelProxy##TypeName)\
	{\
		AVoxelDatabase::VoxelMateVoxelDatabase->FillGrid<FVoxel##TypeName>(GridId, VoxelProxy->IndexCoord, FillDimensions, FVoxel##TypeName(VoxelProxy##TypeName->VoxelValue), VoxelProxy->IsVoxelActive, VoxelizeActiveTilesAfterFill);\
	}\
}\
void AVoxelGridProxy##TypeName::SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface)\
{\
	UVoxel##TypeName##Source* ValueSource = Cast<UVoxel##TypeName##Source>(ValueSourceInterface.GetObject());\
	if (AVoxelDatabase::VoxelMateVoxelDatabase && ValueSource)\
	{\
		if (VoxelIter == EVoxelIterator::InactiveVoxelsIter)\
		{\
			AVoxelDatabase::VoxelMateVoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::InactiveVoxelsIter>(GridId, *ValueSource);\
		}\
		else if (VoxelIter == EVoxelIterator::ActiveVoxelsIter)\
		{\
			AVoxelDatabase::VoxelMateVoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::ActiveVoxelsIter>(GridId, *ValueSource);\
		}\
		else\
		{\
			AVoxelDatabase::VoxelMateVoxelDatabase->SetVoxelValuesFromSource<FVoxel##TypeName, UVoxel##TypeName##Source, EVoxelIterator::AllVoxelsIter>(GridId, *ValueSource);\
		}\
	}\
}

GRID_PROXY_INTERFACE_IMPLEMENTATION(Bool)
GRID_PROXY_INTERFACE_IMPLEMENTATION(Float)
GRID_PROXY_INTERFACE_IMPLEMENTATION(Int32)
GRID_PROXY_INTERFACE_IMPLEMENTATION(UInt8)
GRID_PROXY_INTERFACE_IMPLEMENTATION(Vector)
GRID_PROXY_INTERFACE_IMPLEMENTATION(IntVector)
