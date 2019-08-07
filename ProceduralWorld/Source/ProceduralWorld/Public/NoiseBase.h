// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "NoiseBase.generated.h"

class UWorldChunkStats;
/**
 *
 */
 UCLASS(EditInlineNew)
class PROCEDURALWORLD_API UNoiseBase: public UObject
{
	 GENERATED_BODY()
public:
	virtual int GetNoise(float x, float y, float z, float scale, int32 max);
	virtual int Calculate(UWorldChunkStats* stats, FVector noisePosition);
};
