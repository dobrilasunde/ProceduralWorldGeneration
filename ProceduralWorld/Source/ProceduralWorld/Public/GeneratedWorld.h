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
	void RequestWorldGeneration(FVector origin);
	void CreateWorld();
	Block* GetBlock(int32 x, int32 y, int32 z);
	Block* GetBlockFromWorldPosition(FVector worldPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int32 worldX = 4;
	UPROPERTY(EditAnywhere)
	int32 worldY = 4;
	UPROPERTY(EditAnywhere)
	int32 chunkX = 16;
	UPROPERTY(EditAnywhere)
	int32 chunkY = 16;
	UPROPERTY(EditAnywhere)
	int32 chunkZ = 10;
	UPROPERTY(EditAnywhere)
	float baseNoise = 0.02f;
	UPROPERTY(EditAnywhere)
	int32 baseNoiseHeight = 4;
	UPROPERTY(EditAnywhere)
	int32 elevation = 15;
	UPROPERTY(EditAnywhere)
	float frequency = 0.005f;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UNoiseBase*> noisePatterns;

	int32 maxJobs = 4;
	TArray<WorldGeneration*> toDoJobs;
	TArray<WorldGeneration*> currentJobs;

	FRunnableThread* Thread;

private:
	TArray<TArray<TArray<Block*>>> grid;
	int32 mThreadCounter;
};
/*----------------------------------------------------------------------------------------------------*/
