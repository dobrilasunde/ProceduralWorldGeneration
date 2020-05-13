// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/*----------------------------------------------------------------------------------------------------*/
class WorldGeneration;
class MeshData;

/*----------------------------------------------------------------------------------------------------*/
class PROCEDURALWORLD_API Block
{
public:
	int X;
	int Y;
	int Z;

	bool IsSolid;
	
	FVector LocalPosition;
	FVector WorldPosition;
	
	Block* ParentBlock;

public:
	Block();
	virtual ~Block();

	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;

	virtual void LoadBlock(MeshData* meshData, WorldGeneration* world);

	void SetHCost(float hCost);
	void SetGCost(float gCost);
	void SetFCost(float fCost);
	float GetHCost() const;
	float GetGCost() const;
	float GetFCost() const;

private:
	float _hCost;
	float _gCost;
	float _fCost;
};
/*----------------------------------------------------------------------------------------------------*/