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
	// Sets default values for this actor's properties
	AGeneratedWorld();
	// Called every frame
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
	int32 worldX = 2;
	UPROPERTY(EditAnywhere)
	int32 worldY = 2;
	UPROPERTY(EditAnywhere)
	int32 chunkSizeX = 16;
	UPROPERTY(EditAnywhere)
	int32 chunkSizeY = 16;
	UPROPERTY(EditAnywhere)
	int32 chunkSizeZ = 10;
	UPROPERTY(EditAnywhere)
	float baseNoise = 0.02f;
	UPROPERTY(EditAnywhere)
	int32 baseNoiseHeight = 4;
	UPROPERTY(EditAnywhere)
	int32 elevation = 5;
	UPROPERTY(EditAnywhere)
	float frequency = 0.05f;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UNoiseBase*> noisePatterns;

	int32 maxJobs = 4;
	TArray<WorldGeneration*> toDoJobs;
	TArray<WorldGeneration*> currentJobs;

	FRunnableThread* Thread;

private:
	TArray<TArray<TArray<Chunk*>>> mChunks;
	int32 mThreadCounter;
};
/*----------------------------------------------------------------------------------------------------*/
PROCEDURALWORLD_API AGeneratedWorld* GetGeneratedWorld(const UObject* worldContextObject);
/*----------------------------------------------------------------------------------------------------*/