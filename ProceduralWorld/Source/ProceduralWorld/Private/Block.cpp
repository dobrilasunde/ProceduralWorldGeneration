// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "WorldGeneration.h"
#include "MeshData.h"
#include "MeshCreatorUtilities.h"

Block::Block(): isSolid(false)
{
}

Block::~Block()
{
}

void Block::LoadBlock(MeshData* meshData, WorldGeneration* world)
{
	MeshCreatorUtilities::FaceUp(meshData, localPosition);

	Block* north = world->GetBlock(x + 1, y, z);
	if (north == nullptr || !north->isSolid)
	{
		MeshCreatorUtilities::FaceNorth(meshData, localPosition);
	}
	
	Block* south = world->GetBlock(x - 1, y, z);
	if (south == nullptr || !south->isSolid)
	{
		MeshCreatorUtilities::FaceSouth(meshData, localPosition);
	}

	Block* west = world->GetBlock(x, y - 1, z);
	if (west == nullptr || !west->isSolid)
	{
		MeshCreatorUtilities::FaceWest(meshData, localPosition);
	}

	Block* east = world->GetBlock(x, y + 1, z);
	if (east == nullptr || !east->isSolid)
	{
		MeshCreatorUtilities::FaceEast(meshData, localPosition);
	}
	

}
