// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseNoise.h"
#include "WorldChunkStats.h"
#include "SimplexNoise.h"

int UBaseNoise::Calculate(UWorldChunkStats* stats, FVector noisePosition)
{
	return GetNoise(noisePosition.X / 100, noisePosition.Y / 100, 0, stats->baseNoise, stats->baseNoiseHeight);
}
