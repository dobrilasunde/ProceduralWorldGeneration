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
	Block* GetFirstSolidBlock();

public:
	int32 X;
	int32 Y;
	int32 Z;
	int32 MaxX;
	int32 MaxY;
	int32 MaxZ;

 private:
	Block* GetBlock(int32 x, int32 y, int32 z);

private:
	FVector _chunkPosition;

	TArray<TArray<TArray<Block*>>> _grid;
};
/*----------------------------------------------------------------------------------------------------*/