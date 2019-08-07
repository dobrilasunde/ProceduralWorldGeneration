// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"

class Block;
class UWorldChunkStats;
class MeshData;
class AGeneratedWorld;

DECLARE_DELEGATE_TwoParams(FWorldGenerationCallback, TArray<TArray<TArray<Block*>>>&, MeshData*);

/**
 * 
 */
class PROCEDURALWORLD_API WorldGeneration: public FRunnable
{
public:
	WorldGeneration(UWorldChunkStats* stats, const AGeneratedWorld* generatedWorld);
	~WorldGeneration();
	int32 GetNoise(float x, float y, float z, float scale, int32 max);
	MeshData* CreateChunk();
	Block* GetBlock(int32 x, int32 y, int32 z);
	void NotifyComplete();

	// Begin FRunnable interface.
	virtual uint32 Run();
	// End FRunnable interface

public:
	volatile bool mJobDone;

private:
	TArray<TArray<TArray<Block*>>> mGrid;
	UWorldChunkStats* mStats;
	FWorldGenerationCallback mFinishCallback;
	MeshData* mMeshData;
};
