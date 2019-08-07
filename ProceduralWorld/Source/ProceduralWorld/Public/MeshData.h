// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROCEDURALWORLD_API MeshData
{
public:
	MeshData();
	~MeshData();

public:
	TArray<FVector> mVertices;
	TArray<int32> mTriangles;
	TArray<FVector2D> mUV;
	FVector mOrigin;
};
