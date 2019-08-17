/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
/*----------------------------------------------------------------------------------------------------*/
class Block;
/*----------------------------------------------------------------------------------------------------*/
class PROCEDURALWORLD_API Chunk
{
public:
	Chunk();
	Chunk(int x, int y, int z, int maxX, int maxY, int maxZ, TArray<TArray<TArray<Block*>>> grid);
	Block* GetBlock(FVector worldPosition);

 private:
	Block* GetBlock(int32 x, int32 y, int32 z);

public:
	int32 x;
	int32 y;
	int32 z;

	int32 maxX;
	int32 maxY;
	int32 maxZ;

	FVector chunkPosition;

	TArray<TArray<TArray<Block*>>> grid;
};
/*----------------------------------------------------------------------------------------------------*/