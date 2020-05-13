// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GeneratedWorld.h"
#include "../Mesh/MeshData.h"
#include "../Mesh/MeshCreatorUtilities.h"
#include "../Block/Block.h"
#include "WorldGeneration.h"
#include "WorldChunkStats.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../NoisePasses/NoiseBase.h"
#include "Chunk.h"
#include "../ProceduralWorldGameModeBase.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "../NoisePasses/BaseNoise.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/GodPlayerController.h"
#include "../Units/Unit.h"

/*----------------------------------------------------------------------------------------------------*/
// Sets default values
AGeneratedWorld::AGeneratedWorld(): _threadCounter(0)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterial>material(TEXT("Material'/Game/Textures/M_Tiles.M_Tiles'"));
	if (material.Succeeded()) 
	{
		Material = UMaterialInstanceDynamic::Create(material.Object, material.Object);
	}

	// Testing noises
	//UBaseNoise* baseNoise = NewObject<UBaseNoise>();
	//noisePatterns.Add(baseNoise);
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
	for (int x = 0; x < WorldX + 1; ++x)
	{
		TArray<TArray<Chunk*>> arrayX;
		for (int y = 0; y < WorldY + 1; ++y)
		{
			TArray<Chunk*> arrayY;
			for (int z = 0; z < 1; ++z)
			{
				arrayY.Add(new Chunk());
			}
			arrayX.Add(arrayY);
		}
		Chunks.Add(arrayX);
	}

	for (int x = 0; x < WorldX; ++x)
	{
		for (int y = 0; y < WorldY; ++y)
		{
			RequestWorldGeneration(x, y, ChunkSizeX, ChunkSizeY);
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
// Called every frame
void AGeneratedWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int i = 0;
	while (i < _currentJobs.Num())
	{
		if (_currentJobs[i]->JobDone)
		{
			_currentJobs[i]->NotifyComplete();
			_currentJobs.RemoveAt(i);
		}
		else
		{
			i++;
		}
	}

	if (_toDoJobs.Num() > 0 && _currentJobs.Num() < _maxJobs)
	{
		WorldGeneration* job = _toDoJobs[0];
		_toDoJobs.RemoveAt(0);
		_currentJobs.Add(job);

		_threadCounter++;
		FString threadName = TEXT("WorldGeneration Thread ") + _threadCounter;
		_thread = FRunnableThread::Create(job, *threadName, 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGeneratedWorld::LoadMeshData(TArray<TArray<TArray<Block*>>>& createdGrid, MeshData* meshData)
{
	Chunk* chunk = new Chunk(meshData->chunkPositionX, meshData->chunkPositionY, meshData->chunkPositionZ, ChunkSizeX, ChunkSizeY, ChunkSizeZ, createdGrid);
	Chunks[chunk->X][chunk->Y][chunk->Z] = chunk;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	AActor* emptyActor = GetWorld()->SpawnActor<AActor>(meshData->mOrigin, FRotator::ZeroRotator, spawnParams);

	UProceduralMeshComponent* proceduralMesh = NewObject<UProceduralMeshComponent>(emptyActor);
	proceduralMesh->SetWorldLocation(meshData->mOrigin);
	
	proceduralMesh->RegisterComponentWithWorld(GetWorld());
	emptyActor->SetRootComponent(proceduralMesh);
	
	proceduralMesh->CreateMeshSection_LinearColor(0, meshData->mVertices, meshData->mTriangles, TArray<FVector>(), meshData->mUV, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	proceduralMesh->SetMaterial(0, Material);
	proceduralMesh->ContainsPhysicsTriMeshData(true);

	AGodPlayerController* playerController = Cast<AGodPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!_isUnitPlaced && playerController != nullptr && playerController->Unit != nullptr)
	{
		_isUnitPlaced = true;
		Block* block = chunk->GetFirstSolidBlock();
		FTransform newTransform = playerController->Unit->GetTransform();
		newTransform.SetTranslation(block->WorldPosition + FVector::UpVector * 100.0f);
		playerController->Unit->SetActorTransform(newTransform);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGeneratedWorld::RequestWorldGeneration(int32 x, int32 y, int32 chunkX, int32 chunkY)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating new world!!"));

	UWorldChunkStats* stats = NewObject<UWorldChunkStats>(UWorldChunkStats::StaticClass());
	stats->maxX = ChunkSizeX;
	stats->maxY = ChunkSizeY;
	stats->maxZ = ChunkSizeZ;
	stats->chunkPositionX = x;
	stats->chunkPositionY = y;
	stats->baseNoise = BaseNoise;
	stats->baseNoiseHeight = BaseNoiseHeight;
	stats->elevation = Elevation;
	stats->frequency = Frequency;
	stats->mOrigin = FVector(x * ChunkSizeX * 100.0f, y * ChunkSizeY * 100.0f, 0.0f);
	stats->mNoisePatterns = NoisePatterns;

	WorldGeneration* worldGeneration = new WorldGeneration(stats, this);
	_toDoJobs.Add(worldGeneration);
}
/*----------------------------------------------------------------------------------------------------*/
Block* AGeneratedWorld::GetBlockFromWorldPosition(FVector worldPosition)
{
	int chunkPositionX = FMath::FloorToInt(worldPosition.X / (100.0f * ChunkSizeX));
	int chunkPositionY = FMath::FloorToInt(worldPosition.Y / (100.0f * ChunkSizeY));
	int chunkPositionZ = FMath::FloorToInt(worldPosition.Z / (100.0f * ChunkSizeZ));

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
	if (x < 0 || y < 0 || z < 0 || x > WorldX - 1 || y > WorldY - 1 || z > 0)
	{
		return nullptr;
	}
	
	return Chunks[x][y][z];
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
