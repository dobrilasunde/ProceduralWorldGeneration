// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"

class MeshData;
class Tile;

static class PROCEDURALWORLD_API MeshCreatorUtilities
{
public:
	static void FaceUp(MeshData* meshData, FVector origin);
	static void FaceDown(MeshData* meshData, FVector origin);
	static void FaceNorth(MeshData* meshData, FVector origin);
	static void FaceEast(MeshData* meshData, FVector origin);
	static void FaceWest(MeshData* meshData, FVector origin);
	static void FaceSouth(MeshData* meshData, FVector origin);
	static void AddTriangles(MeshData* meshData);
	static void AddUVs(MeshData* meshData, Direction direction = Direction::Other);
	static Tile* TexturePosition(Direction direction);

public:
	static const float mTileScale;
};
