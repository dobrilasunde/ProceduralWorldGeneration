// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "PathfinderMaster.h"

/*----------------------------------------------------------------------------------------------------*/
class Block;
class Chunk;
class AGeneratedWorld;
struct FVector;
class AUnit;
/*----------------------------------------------------------------------------------------------------*/
class PROCEDURALWORLD_API Pathfinder: public FRunnable
{
public:
	Pathfinder(AGeneratedWorld* world, Block* start, Block* target, AUnit* unit, FPathCompleteCallback& callback);
	~Pathfinder();

	void FindPath();
	TArray<Block*> FindActualPath(Block* start, Block* target);
	TArray<Block*> RetracePath(Block* start, Block* current);
	TArray<Block*> GetNeighbours(Block* block);
	Block* GetNeighbour(Block* search, Block* currentBlock);
	
	void NotifyComplete();

	virtual uint32 Run();

	int GetDistance(Block* from, Block* to);
	Block* GetBlock(FVector worldPosition);
	Block* GetBlockFromWorldPosition(FVector worldPosition);
	Chunk* GetChunk(int32 x, int32 y, int32 z);

public:
	volatile bool JobDone;

private:
	Block* _startBlock;
	Block* _endBlock;

	TArray<TArray<TArray<Chunk*>>> _chunks;

	TArray<Block*> _path;

	int32 _worldX = 2;
	int32 _worldY = 2;
	int32 _chunkSizeX = 16;
	int32 _chunkSizeY = 16;
	int32 _chunkSizeZ = 10;

	FPathCompleteCallback _pathCompleteCallback;

	TWeakObjectPtr<AUnit> _unit;
};
/*----------------------------------------------------------------------------------------------------*/