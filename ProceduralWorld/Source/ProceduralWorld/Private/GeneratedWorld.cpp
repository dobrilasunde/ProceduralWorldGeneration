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
#include "Chunk.h"
#include "ProceduralWorldGameModeBase.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "BaseNoise.h"
/*----------------------------------------------------------------------------------------------------*/
// Sets default values
AGeneratedWorld::AGeneratedWorld(): mThreadCounter(0)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Textures/M_Tiles.M_Tiles'"));
	if (Material.Succeeded()) 
	{
		material = UMaterialInstanceDynamic::Create(Material.Object, Material.Object);
	}

	// Testing noises
	UBaseNoise* baseNoise = NewObject<UBaseNoise>();
	noisePatterns.Add(baseNoise);
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
	// Initializing "3D array".
	for (int x = 0; x < worldX + 1; ++x)
	{
		TArray<TArray<Chunk*>> arrayX;
		for (int y = 0; y < worldY + 1; ++y)
		{
			TArray<Chunk*> arrayY;
			for (int z = 0; z < 1; ++z)
			{
				arrayY.Add(new Chunk());
			}
			arrayX.Add(arrayY);
		}
		mChunks.Add(arrayX);
	}

	for (int x = 0; x < worldX; ++x)
	{
		for (int y = 0; y < worldY; ++y)
		{
			RequestWorldGeneration(x, y, chunkSizeX, chunkSizeY);
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
	Chunk* chunk = new Chunk(meshData->chunkPositionX, meshData->chunkPositionY, meshData->chunkPositionZ, chunkSizeX, chunkSizeY, chunkSizeZ, createdGrid);
	mChunks[chunk->x][chunk->y][chunk->z] = chunk;

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
void AGeneratedWorld::RequestWorldGeneration(int32 x, int32 y, int32 chunkX, int32 chunkY)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating new world!!"));

	UWorldChunkStats* stats = NewObject<UWorldChunkStats>(UWorldChunkStats::StaticClass());
	stats->maxX = chunkSizeX;
	stats->maxY = chunkSizeY;
	stats->maxZ = chunkSizeZ;
	stats->chunkPositionX = x;
	stats->chunkPositionY = y;
	stats->baseNoise = baseNoise;
	stats->baseNoiseHeight = baseNoiseHeight;
	stats->elevation = elevation;
	stats->frequency = frequency;
	stats->mOrigin = FVector(x * chunkSizeX * 100.0f, y * chunkSizeY * 100.0f, 0.0f);
	stats->mNoisePatterns = noisePatterns;

	WorldGeneration* worldGeneration = new WorldGeneration(stats, this);
	toDoJobs.Add(worldGeneration);
}
/*----------------------------------------------------------------------------------------------------*/
Block* AGeneratedWorld::GetBlockFromWorldPosition(FVector worldPosition)
{
	int chunkPositionX = FMath::FloorToInt(worldPosition.X / (100.0f * chunkSizeX));
	int chunkPositionY = FMath::FloorToInt(worldPosition.Y / (100.0f * chunkSizeY));
	int chunkPositionZ = FMath::FloorToInt(worldPosition.Z / (100.0f * chunkSizeZ));

	Chunk* chunk = GetChunk(chunkPositionX, chunkPositionY, chunkPositionZ);

	if (chunk == nullptr)
	{
		return nullptr;
	}

	return chunk->GetBlock(worldPosition);
}
/*----------------------------------------------------------------------------------------------------*/
Chunk* AGeneratedWorld::GetChunk(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > worldX || y > worldY || z > 0)
	{
		return nullptr;
	}
	
	return mChunks[x][y][z];
}
/*----------------------------------------------------------------------------------------------------*/
AGeneratedWorld* GetGeneratedWorld(const UObject* worldContextObject)
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
	return gameMode->GetGeneratedWorld();
}
/*----------------------------------------------------------------------------------------------------*/
