// Fill out your copyright notice in the Description page of Project Settings.

#include "PathfinderMaster.h"
#include "Pathfinder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "../World/GeneratedWorld.h"
#include "../Block/Block.h"
#include "../ProceduralWorldGameModeBase.h"
#include <DrawDebugHelpers.h>
#include "../Units/Unit.h"
#include "Algo/Reverse.h"

//--------------------------------------------------------------------------------------------------
APathfinderMaster::APathfinderMaster()
{
	PrimaryActorTick.bCanEverTick = true;
}
//--------------------------------------------------------------------------------------------------
void APathfinderMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int i = 0;
	while (i < _currentJobs.Num())
	{
		if (_currentJobs[i]->JobDone)
		{
			_currentJobs[i]->NotifyComplete();
			_currentJobs.RemoveAt(i);
		}
		else
		{
			i++;
		}
	}

	if (_toDoJobs.Num() > 0 && _currentJobs.Num() < _maxJobs)
	{
		Pathfinder* job = _toDoJobs[0];
		_toDoJobs.RemoveAt(0);
		_currentJobs.Add(job);

		_threadCounter++;
		FString threadName = TEXT("Pathfinding Thread ") + _threadCounter;
		_thread = FRunnableThread::Create(job, *threadName, 0, TPri_BelowNormal);
	}
}
//--------------------------------------------------------------------------------------------------
void APathfinderMaster::RequestPathfind(Block* start, Block* target, AUnit* unit)
{
	if (start == nullptr || target == nullptr || unit == nullptr)
	{
		return;
	}

	FPathCompleteCallback pathCompleteCallback = FPathCompleteCallback::CreateUObject(this, &APathfinderMaster::PathCallback);
	Pathfinder* pathJob = new Pathfinder(GetGeneratedWorld(this), start, target, unit, pathCompleteCallback);
	_toDoJobs.Add(pathJob);
}
//--------------------------------------------------------------------------------------------------
void APathfinderMaster::PathCallback(TArray<Block*> path, AUnit* unit)
{
	if (unit == nullptr)
	{
		return;
	}
	
	Algo::Reverse(path);

	unit->LoadPath(path);

	if (path.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created path!"));
		for (int32 i = 0; i < path.Num(); ++i)
		{
			DrawDebugSphere(GetWorld(), path[i]->WorldPosition + FVector::UpVector * 100.0f, 30.0f, 16.0f, FColor::Magenta, true, 5.0f);
		}
	}
}
//--------------------------------------------------------------------------------------------------
PROCEDURALWORLD_API APathfinderMaster* GetPathfinderMaster(const UObject* worldContextObject)
{
	if (worldContextObject == nullptr)
	{
		return nullptr;
	}
	UWorld* world = worldContextObject->GetWorld();
	if (world == nullptr)
	{
		return nullptr;
	}
	AProceduralWorldGameModeBase* gameMode = Cast<AProceduralWorldGameModeBase>(world->GetAuthGameMode());
	if (gameMode == nullptr)
	{
		return nullptr;
	}
	return gameMode->GetPathfinderMaster();
}
//--------------------------------------------------------------------------------------------------