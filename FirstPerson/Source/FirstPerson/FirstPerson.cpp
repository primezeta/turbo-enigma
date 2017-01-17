// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FirstPerson.h"
#include "VoxelDatabaseTesting.h"

DEFINE_LOG_CATEGORY(LogFirstPerson);

void UVoxelPerlinNoiseSource::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVoxelPerlinNoiseSource, Frequency);
	DOREPLIFETIME(UVoxelPerlinNoiseSource, Lacunarity);
	DOREPLIFETIME(UVoxelPerlinNoiseSource, Persistence);
	DOREPLIFETIME(UVoxelPerlinNoiseSource, OctaveCount);
}

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, FirstPerson, "FirstPerson" );
 