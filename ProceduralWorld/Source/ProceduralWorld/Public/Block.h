// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class WorldGeneration;
class MeshData;

/**
 * 
 */
class PROCEDURALWORLD_API Block
{
public:
	Block();
	virtual ~Block();
	virtual void LoadBlock(MeshData* meshData, WorldGeneration* world);

public:
	int x;
	int y;
	int z;
	bool isSolid;
	FVector worldPosition;
};
