// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGeneration.h"
#include "SimplexNoise.h"
#include "MeshData.h"
#include "GeneratedWorld.h"
#include "WorldChunkStats.h"
#include "Block.h"
#include "NoiseBase.h"
#include "BaseNoise.h"
WorldGeneration::WorldGeneration(UWorldChunkStats* stats, const AGeneratedWorld* generatedWorld): mJobDone(false)
{
	mStats = stats;
	mFinishCallback.BindUObject(generatedWorld, &AGeneratedWorld::LoadMeshData);
}

WorldGeneration::~WorldGeneration()
{
}

uint32 WorldGeneration::Run()
{
	mMeshData = CreateChunk();
	mJobDone = true;

	return 0;
}

void WorldGeneration::NotifyComplete()
{
	mFinishCallback.Execute(mGrid, mMeshData);
}

int32 WorldGeneration::GetNoise(float x, float y, float z, float scale, int32 max)
{
	return FMath::FloorToInt((SimplexNoise::noise(x * scale, y * scale, z * scale) + 1) * (max / 2.0f));
}

MeshData* WorldGeneration::CreateChunk()
{
	// Initializing "3D array".
	for (int x = 0; x < mStats->maxX; ++x)
	{
		TArray<TArray<Block*>> arrayX;
		for (int y = 0; y < mStats->maxY; ++y)
		{
			TArray<Block*> arrayY;
			for (int z = 0; z < mStats->maxZ; ++z)
			{
				arrayY.Add(new Block());
			}
			arrayX.Add(arrayY);
		}
		mGrid.Add(arrayX);
	}

	TArray<Block*> blocks;
	for (int x = 0; x < mStats->maxX; ++x)
	{
		for (int y = 0; y < mStats->maxY; ++y)
		{
			FVector targetPosition = FVector::ZeroVector;
			targetPosition.X += x * 100;
			targetPosition.Y += y * 100;
			float height = 0;

			FVector noisePosition = mStats->mOrigin;
			noisePosition.X += targetPosition.X;
			noisePosition.Y += targetPosition.Y;

			if (mStats->mNoisePatterns.Num() > 0)
			{
				for (int i = 0; i < mStats->mNoisePatterns.Num(); ++i)
				{
					UNoiseBase* n = mStats->mNoisePatterns[i];
					height += n->Calculate(mStats, noisePosition);

				}
			}

			if (height > mStats->maxY)
			{
				continue;
			}

			targetPosition.Z += height * 100;
			mGrid[x][y][FMath::FloorToInt(height)]->worldPosition = targetPosition;

			mGrid[x][y][FMath::FloorToInt(height)]->x = x;
			mGrid[x][y][FMath::FloorToInt(height)]->y = y;
			mGrid[x][y][FMath::FloorToInt(height)]->z = FMath::FloorToInt(height);
			mGrid[x][y][FMath::FloorToInt(height)]->isSolid = true;

			blocks.Add(mGrid[x][y][FMath::FloorToInt(height)]);
		}
	}

	MeshData* meshData = new MeshData();
	for (int i = 0; i < blocks.Num(); ++i)
	{
		blocks[i]->LoadBlock(meshData, this);
	}
	meshData->mOrigin = mStats->mOrigin;
	return meshData;
}

Block* WorldGeneration::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > mStats->maxX - 1 || y > mStats->maxY - 1 || z > mStats->elevation - 1)
	{
		return nullptr;
	}

	return mGrid[x][y][z];
}

