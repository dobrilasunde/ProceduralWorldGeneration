// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinder.h"
#include "../World/Chunk.h"
#include "../Block/Block.h"
#include "../World/GeneratedWorld.h"
#include "Math/Vector.h"
#include "../Units/Unit.h"

//--------------------------------------------------------------------------------------------------
Pathfinder::Pathfinder(AGeneratedWorld* world, Block* start, Block* target, AUnit* unit, FPathCompleteCallback& callback): _startBlock(start), _endBlock(target), _unit(unit)
{
	_pathCompleteCallback = callback;
	JobDone = false;

	if (world != nullptr)
	{
		_worldX = world->WorldX;
		_worldY = world->WorldY;
		_chunkSizeX = world->ChunkSizeX;
		_chunkSizeY = world->ChunkSizeY;
		_chunkSizeZ = world->ChunkSizeZ;

		_chunks = world->Chunks;
	}
}
//--------------------------------------------------------------------------------------------------
Pathfinder::~Pathfinder()
{

}
//--------------------------------------------------------------------------------------------------
void Pathfinder::FindPath()
{
	_path = FindActualPath(_startBlock, _endBlock);
	JobDone = true;
}
//--------------------------------------------------------------------------------------------------
TArray<Block*> Pathfinder::FindActualPath(Block* start, Block* target)
{
	TArray<Block*> result;
	if (start == nullptr || target == nullptr)
	{
		return result;
	}

	TArray<Block*> openSet;
	TSet<Block*> closedSet;

	openSet.Add(start);

	while (openSet.Num() > 0)
	{
		Block* currentBlock = openSet[0];

		for (int32 i = 0; i < openSet.Num(); ++i)
		{
			if (openSet[i]->GetFCost() < currentBlock->GetFCost() || 
				(openSet[i]->GetFCost() == currentBlock->GetFCost() && openSet[i]->GetHCost() < currentBlock->GetHCost()))
			{
				if (currentBlock != openSet[i])
				{
					currentBlock = openSet[i];
				}
			}
		}

		openSet.Remove(currentBlock);
		closedSet.Add(currentBlock);

		if (currentBlock == target)
		{
			result = RetracePath(start, currentBlock);
			break;
		}

		for (Block* block : GetNeighbours(currentBlock))
		{
			if (!closedSet.Contains(block))
			{
				float moveCost = currentBlock->GetGCost() + GetDistance(currentBlock, block);

				if (moveCost < block->GetGCost() || !openSet.Contains(block))
				{
					block->SetGCost(moveCost);
					block->SetHCost(GetDistance(block, target));
					block->ParentBlock = currentBlock;

					if (!openSet.Contains(block))
					{
						openSet.Add(block);
					}
				}
			}
		}
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
TArray<Block*> Pathfinder::RetracePath(Block* start, Block* current)
{
	TArray<Block*> path;
	if (start == nullptr || current == nullptr)
	{
		return path;
	}

	Block* currentBlock = _endBlock;

	while (currentBlock != start)
	{
		path.Add(currentBlock);
		currentBlock = currentBlock->ParentBlock;
	}

	return path;
}
//--------------------------------------------------------------------------------------------------
TArray<Block*> Pathfinder::GetNeighbours(Block* block)
{
	TArray<Block*> result;
	if (block == nullptr)
	{
		return result;
	}

	for (int32 x = -1; x <= 1; ++x)
	{
		for (int32 y = -1; y <= 1; ++y)
		{
			for (int32 z = -1; z <= 1; ++z)
			{
				FVector targetPosition = FVector::ZeroVector;
				targetPosition.X = x * 100.0f;
				targetPosition.Y = y * 100.0f;
				targetPosition.Z = z * 100.0f;

				targetPosition += block->WorldPosition;

				Block* searchPosition = GetBlockFromWorldPosition(targetPosition);
				if (searchPosition != nullptr)
				{
					Block* pathTarget = GetNeighbour(searchPosition, block);

					if (pathTarget != nullptr)
					{
						result.Add(pathTarget);
					}
				}
			}
		}
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
Block* Pathfinder::GetNeighbour(Block* search, Block* currentBlock)
{
// 	if (search == nullptr || currentBlock == nullptr)
// 	{
// 		return nullptr;
// 	}
// 	
// 	int32 originalX = search->X - currentBlock->X;
// 	int32 originalY = search->Y - currentBlock->Y;
// 
// 	if (originalX != 0 && originalY != 0)
// 	{
// 		return nullptr;
// 	}

	return search;
}
//--------------------------------------------------------------------------------------------------
void Pathfinder::NotifyComplete()
{
	if (_pathCompleteCallback.IsBound())
	{
		_pathCompleteCallback.Execute(_path, _unit.Get());
	}
}
//--------------------------------------------------------------------------------------------------
uint32 Pathfinder::Run()
{
	FindPath();

	return 0;
}
//--------------------------------------------------------------------------------------------------
int Pathfinder::GetDistance(Block* from, Block* to)
{
	if (from == nullptr || to == nullptr)
	{
		return 0.0f;
	}

	int distanceX = FMath::Abs(from->X - to->X);
	int distanceY = FMath::Abs(from->Y - to->Y);
	int distanceZ = FMath::Abs(from->Z - to->Z);

	if (distanceX > distanceY)
	{
		return 14.0f * distanceY + 10 * (distanceX - distanceY) + 10 * distanceZ;
	}

	return 14.0f * distanceX + 10 * (distanceY - distanceX) * distanceZ;
}
//--------------------------------------------------------------------------------------------------
Block* Pathfinder::GetBlock(FVector worldPosition)
{
	return GetBlockFromWorldPosition(worldPosition);
}
//--------------------------------------------------------------------------------------------------
Block* Pathfinder::GetBlockFromWorldPosition(FVector worldPosition)
{
	int chunkPositionX = FMath::FloorToInt(worldPosition.X / (100.0f * _chunkSizeX));
	int chunkPositionY = FMath::FloorToInt(worldPosition.Y / (100.0f * _chunkSizeY));
	int chunkPositionZ = FMath::FloorToInt(worldPosition.Z / (100.0f * _chunkSizeZ));

	Chunk* chunk = GetChunk(chunkPositionX, chunkPositionY, chunkPositionZ);

	if (chunk == nullptr)
	{
		return nullptr;
	}

	return chunk->GetBlock(worldPosition);
}
//--------------------------------------------------------------------------------------------------
Chunk* Pathfinder::GetChunk(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > _worldX - 1 || y > _worldY - 1 || z > 0)
	{
		return nullptr;
	}

	return _chunks[x][y][z];
}
//--------------------------------------------------------------------------------------------------