// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralWorldGameModeBase.h"
#include "GeneratedWorld.h"
/*----------------------------------------------------------------------------------------------------*/
AProceduralWorldGameModeBase::AProceduralWorldGameModeBase()
{
	_generatedWorldClass = AGeneratedWorld::StaticClass();
}
/*----------------------------------------------------------------------------------------------------*/
void AProceduralWorldGameModeBase::InitGame(const FString& mapName, const FString& options, FString& errorMessage)
{
	Super::InitGame(mapName, options, errorMessage);

	if (_generatedWorldClass != nullptr)
	{
		_generatedWorld = GetWorld()->SpawnActor<AGeneratedWorld>(_generatedWorldClass);
	}
}
/*----------------------------------------------------------------------------------------------------*/
AGeneratedWorld* AProceduralWorldGameModeBase::GetGeneratedWorld() const
{
	return _generatedWorld;
}
/*----------------------------------------------------------------------------------------------------*/
