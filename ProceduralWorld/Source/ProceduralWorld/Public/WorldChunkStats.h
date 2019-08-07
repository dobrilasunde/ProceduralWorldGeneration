// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorldChunkStats.generated.h"

class UNoiseBase;

UCLASS(BlueprintType)
class PROCEDURALWORLD_API UWorldChunkStats: public UObject
{
	
	GENERATED_BODY()

public:
	int32 maxX = 16;
	int32 maxY = 16;
	int32 maxZ = 10;
	float baseNoise = 0.02f;
	int32 baseNoiseHeight = 4;
	int32 elevation = 15;
	float frequency = 0.005f;
	FVector mOrigin;

	TArray<UNoiseBase*> mNoisePatterns;
};
