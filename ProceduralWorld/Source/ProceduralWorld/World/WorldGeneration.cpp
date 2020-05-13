// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "WorldGeneration.h"
#include "SimplexNoise.h"
#include "../Mesh/MeshData.h"
#include "GeneratedWorld.h"
#include "WorldChunkStats.h"
#include "../Block/Block.h"
#include "../NoisePasses/NoiseBase.h"
#include "../NoisePasses/BaseNoise.h"
/*----------------------------------------------------------------------------------------------------*/
WorldGeneration::WorldGeneration(UWorldChunkStats* stats, const AGeneratedWorld* generatedWorld): JobDone(false)
{
	_stats = stats;
	_finishCallback.BindUObject(generatedWorld, &AGeneratedWorld::LoadMeshData);
}
/*----------------------------------------------------------------------------------------------------*/
WorldGeneration::~WorldGeneration()
{
}
/*----------------------------------------------------------------------------------------------------*/
uint32 WorldGeneration::Run()
{
	_meshData = CreateChunk();
	_meshData->chunkPositionX = _stats->chunkPositionX;
	_meshData->chunkPositionY = _stats->chunkPositionY;
	_meshData->chunkPositionZ = _stats->chunkPositionZ;

	JobDone = true;

	return 0;
}
/*----------------------------------------------------------------------------------------------------*/
void WorldGeneration::NotifyComplete()
{
	_finishCallback.Execute(_grid, _meshData);
}
/*----------------------------------------------------------------------------------------------------*/
int32 WorldGeneration::GetNoise(float x, float y, float z, float scale, int32 max)
{
	return FMath::FloorToInt((SimplexNoise::noise(x * scale, y * scale, z * scale) + 1) * (max / 2.0f));
}
/*----------------------------------------------------------------------------------------------------*/
MeshData* WorldGeneration::CreateChunk()
{
	// Initializing "3D array".
	for (int x = 0; x < _stats->maxX; ++x)
	{
		TArray<TArray<Block*>> arrayX;
		for (int y = 0; y < _stats->maxY; ++y)
		{
			TArray<Block*> arrayY;
			for (int z = 0; z < _stats->maxZ; ++z)
			{
				arrayY.Add(new Block());
			}
			arrayX.Add(arrayY);
		}
		_grid.Add(arrayX);
	}

	TArray<Block*> blocks;
	for (int x = 0; x < _stats->maxX; ++x)
	{
		for (int y = 0; y < _stats->maxY; ++y)
		{
			FVector targetPosition = FVector::ZeroVector;
			targetPosition.X += x * 100;
			targetPosition.Y += y * 100;
			float height = 0;

			FVector noisePosition = _stats->mOrigin;
			noisePosition.X += targetPosition.X;
			noisePosition.Y += targetPosition.Y;

			if (_stats->mNoisePatterns.Num() > 0)
			{
				for (int i = 0; i < _stats->mNoisePatterns.Num(); ++i)
				{
					UNoiseBase* n = _stats->mNoisePatterns[i];
					if (n != nullptr)
					{
						height += n->Calculate(_stats, noisePosition);
					}
				}
			}

			if (height > _stats->maxY)
			{
				continue;
			}

			targetPosition.Z += height * 100;
			_grid[x][y][FMath::FloorToInt(height)]->LocalPosition = targetPosition;
			_grid[x][y][FMath::FloorToInt(height)]->WorldPosition = targetPosition + _stats->mOrigin;

			_grid[x][y][FMath::FloorToInt(height)]->X = x;
			_grid[x][y][FMath::FloorToInt(height)]->Y = y;
			_grid[x][y][FMath::FloorToInt(height)]->Z = FMath::FloorToInt(height);
			_grid[x][y][FMath::FloorToInt(height)]->IsSolid = true;

			blocks.Add(_grid[x][y][FMath::FloorToInt(height)]);
		}
	}

	MeshData* meshData = new MeshData();
	for (int i = 0; i < blocks.Num(); ++i)
	{
		blocks[i]->LoadBlock(meshData, this);
	}
	meshData->mOrigin = _stats->mOrigin;
	return meshData;
}
/*----------------------------------------------------------------------------------------------------*/
Block* WorldGeneration::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > _stats->maxX - 1 || y > _stats->maxY - 1 || z > _stats->elevation - 1)
	{
		return nullptr;
	}

	return _grid[x][y][z];
}
/*----------------------------------------------------------------------------------------------------*/
