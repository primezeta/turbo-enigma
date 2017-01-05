// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FirstPerson.h"
#include "VoxelDatabaseTesting.h"

DEFINE_LOG_CATEGORY(LogFirstPerson);

void AVoxelPerlinNoiseSource::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelPerlinNoiseSource, Frequency);
	DOREPLIFETIME(AVoxelPerlinNoiseSource, Lacunarity);
	DOREPLIFETIME(AVoxelPerlinNoiseSource, Persistence);
	DOREPLIFETIME(AVoxelPerlinNoiseSource, OctaveCount);
}

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, FirstPerson, "FirstPerson" );
 