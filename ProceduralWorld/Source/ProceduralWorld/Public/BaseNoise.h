// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NoiseBase.h"
#include "BaseNoise.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class PROCEDURALWORLD_API UBaseNoise: public UNoiseBase
{
	GENERATED_BODY()
public:
	int Calculate(UWorldChunkStats* stats, FVector noisePosition) override;
};
