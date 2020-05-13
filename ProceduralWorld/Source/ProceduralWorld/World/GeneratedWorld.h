// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GeneratedWorld.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class WorldGeneration;
class MeshData;
class Block;
class UNoiseBase;
class Chunk;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API AGeneratedWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	AGeneratedWorld();
	virtual void Tick(float DeltaTime) override;

	void LoadMeshData(TArray<TArray<TArray<Block*>>>& createdGrid, MeshData* meshData);

	void RequestWorldGeneration(int32 x, int32 y, int32 chunkX, int32 chunkY);
	void CreateWorld();

	Block* GetBlockFromWorldPosition(FVector worldPosition);
	Chunk* GetChunk(int32 x, int32 y, int32 z);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int32 WorldX = 2;
	UPROPERTY(EditAnywhere)
	int32 WorldY = 2;

	UPROPERTY(EditAnywhere)
	int32 ChunkSizeX = 16;
	UPROPERTY(EditAnywhere)
	int32 ChunkSizeY = 16;
	UPROPERTY(EditAnywhere)
	int32 ChunkSizeZ = 10;

	UPROPERTY(EditAnywhere)
	float BaseNoise = 0.02f;
	UPROPERTY(EditAnywhere)
	int32 BaseNoiseHeight = 4;
	UPROPERTY(EditAnywhere)
	int32 Elevation = 5;
	UPROPERTY(EditAnywhere)
	float Frequency = 0.05f;

	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UNoiseBase*> NoisePatterns;

	TArray<TArray<TArray<Chunk*>>> Chunks;

private:
	int32 _threadCounter;

	int32 _maxJobs = 4;
	TArray<WorldGeneration*> _toDoJobs;
	TArray<WorldGeneration*> _currentJobs;

	FRunnableThread* _thread;

	bool _isUnitPlaced;
};
/*----------------------------------------------------------------------------------------------------*/
PROCEDURALWORLD_API AGeneratedWorld* GetGeneratedWorld(const UObject* worldContextObject);
/*----------------------------------------------------------------------------------------------------*/