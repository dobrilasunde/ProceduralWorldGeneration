// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfinderMaster.generated.h"

/*----------------------------------------------------------------------------------------------------*/
class Pathfinder;
class AGeneratedWorld;
class Block;
class AUnit;
/*----------------------------------------------------------------------------------------------------*/
DECLARE_DELEGATE_TwoParams(FPathCompleteCallback, TArray<Block*>, AUnit*);
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API APathfinderMaster : public AActor
{
	GENERATED_BODY()

public:
	APathfinderMaster();
	virtual void Tick(float DeltaTime) override;
	void RequestPathfind(Block* start, Block* target, AUnit* unit);

private:
	void PathCallback(TArray<Block*> path, AUnit* unit);

private:
	int32 _threadCounter;

	int32 _maxJobs = 3;
	TArray<Pathfinder*> _toDoJobs;
	TArray<Pathfinder*> _currentJobs;

	FRunnableThread* _thread;
};
/*----------------------------------------------------------------------------------------------------*/
PROCEDURALWORLD_API APathfinderMaster* GetPathfinderMaster(const UObject* worldContextObject);
/*----------------------------------------------------------------------------------------------------*/