// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GeneratedWorld.h"
#include "MeshData.h"
#include "MeshCreatorUtilities.h"
#include "Block.h"
#include "WorldGeneration.h"
#include "WorldChunkStats.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "NoiseBase.h"
/*----------------------------------------------------------------------------------------------------*/
// Sets default values
AGeneratedWorld::AGeneratedWorld(): mThreadCounter(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
/*----------------------------------------------------------------------------------------------------*/
// Called when the game starts or when spawned
void AGeneratedWorld::BeginPlay()
{
	Super::BeginPlay();

	CreateWorld();
}
/*----------------------------------------------------------------------------------------------------*/
void AGeneratedWorld::CreateWorld()
{
	for (int x = 0; x < worldX; ++x)
	{
		for (int y = 0; y < worldY; ++y)
		{
			FVector startingPosition = FVector::ZeroVector;
			startingPosition.X = x * chunkX * 100;
			startingPosition.Y = y * chunkY * 100;

			RequestWorldGeneration(startingPosition);
		}
	}

}
/*----------------------------------------------------------------------------------------------------*/
// Called every frame
void AGeneratedWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int i = 0;
	while (i < currentJobs.Num())
	{
		if (currentJobs[i]->mJobDone)
		{
			currentJobs[i]->NotifyComplete();
			currentJobs.RemoveAt(i);
		}
		else
		{
			i++;
		}
	}

	if (toDoJobs.Num() > 0 && currentJobs.Num() < maxJobs)
	{
		WorldGeneration* job = toDoJobs[0];
		toDoJobs.RemoveAt(0);
		currentJobs.Add(job);

		mThreadCounter++;
		FString threadName = TEXT("WorldGeneration Thread ") + mThreadCounter;
		Thread = FRunnableThread::Create(job, *threadName, 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGeneratedWorld::LoadMeshData(TArray<TArray<TArray<Block*>>>& createdGrid, MeshData* meshData)
{
	grid = createdGrid;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	AActor* emptyActor = GetWorld()->SpawnActor<AActor>(meshData->mOrigin, FRotator::ZeroRotator, spawnParams);

	UProceduralMeshComponent* proceduralMesh = NewObject<UProceduralMeshComponent>(emptyActor);
	proceduralMesh->SetWorldLocation(meshData->mOrigin);
	
	proceduralMesh->RegisterComponentWithWorld(GetWorld());
	emptyActor->SetRootComponent(proceduralMesh);
	
	proceduralMesh->CreateMeshSection_LinearColor(0, meshData->mVertices, meshData->mTriangles, TArray<FVector>(), meshData->mUV, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	proceduralMesh->SetMaterial(0, material);
	proceduralMesh->ContainsPhysicsTriMeshData(true);	
}
/*----------------------------------------------------------------------------------------------------*/
void AGeneratedWorld::RequestWorldGeneration(FVector origin)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating new world!!"));

	UWorldChunkStats* stats = NewObject<UWorldChunkStats>(UWorldChunkStats::StaticClass());
	stats->maxX = chunkX;
	stats->maxY = chunkY;
	stats->maxZ = chunkZ;
	stats->baseNoise = baseNoise;
	stats->baseNoiseHeight = baseNoiseHeight;
	stats->elevation = elevation;
	stats->frequency = frequency;
	stats->mOrigin = origin;
	stats->mNoisePatterns = noisePatterns;

	WorldGeneration* worldGeneration = new WorldGeneration(stats, this);
	toDoJobs.Add(worldGeneration);
}
/*----------------------------------------------------------------------------------------------------*/
Block* AGeneratedWorld::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > chunkX - 1 || y > chunkY - 1 || z > elevation - 1)
	{
		return nullptr;
	}

	return grid[x][y][z];
}
/*----------------------------------------------------------------------------------------------------*/
Block* GetBlockFromWorldPosition(FVector worldPosition)
{
	return nullptr;
}
/*----------------------------------------------------------------------------------------------------*/
