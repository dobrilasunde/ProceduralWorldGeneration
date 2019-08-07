// Fill out your copyright notice in the Description page of Project Settings.

#include "NoiseBase.h"
#include "SimplexNoise.h"

int UNoiseBase::GetNoise(float x, float y, float z, float scale, int32 max)
{
	return FMath::FloorToInt((SimplexNoise::noise(x * scale, y * scale, z * scale) + 1) * (max / 2.0f));
}

int UNoiseBase::Calculate(UWorldChunkStats* stats, FVector noisePosition)
{
	return -1;
}

