// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelDatabaseProxy.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

class FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    FORCEINLINE static FVoxelMateModule& GetChecked()
    {
        return FModuleManager::LoadModuleChecked<FVoxelMateModule>("VoxelMateModule");
    }

    FORCEINLINE static bool IsLoaded()
    {
        return FModuleManager::GetModulePtr<FVoxelMateModule>("VoxelMateModule") != nullptr;
    }

    void SerializeDatabase(FArchive& Ar);
    
    //template<typename ValueType>
    //FORCEINLINE const ValueType& GetVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord);

    //template<typename ValueType>
    //FORCEINLINE const bool GetVoxelIsActive(const FGuid& GridId, const FIntVector& IndexCoord);

    //template<typename ValueType>
    //FORCEINLINE const ValueType& GetVoxelValueAndIsActive(const FGuid& GridId, const FIntVector& IndexCoord, bool& OutIsActive);

    //template<typename ValueType>
    //FORCEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const ValueType& InValue);

    //template<typename ValueType>
    //FORCEINLINE void SetVoxelIsActive(const FGuid& GridId, const FIntVector& IndexCoord, const bool& InIsActive);

    //template<typename ValueType>
    //FORCEINLINE void SetVoxelValueAndIsActive(const FGuid& GridId, const FIntVector& IndexCoord, const ValueType& InValue, const bool& InIsActive);

    UFUNCTION()
        UVoxelDatabaseProxy* OpenDatabaseProxy();
};
