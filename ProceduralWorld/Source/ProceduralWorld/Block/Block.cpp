// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "../World/WorldGeneration.h"
#include "../Mesh/MeshData.h"
#include "../Mesh/MeshCreatorUtilities.h"

/*----------------------------------------------------------------------------------------------------*/
Block::Block(): IsSolid(false)
{
}
/*----------------------------------------------------------------------------------------------------*/
Block::~Block()
{
}
//--------------------------------------------------------------------------------------------------
bool Block::operator==(const Block& other) const
{
	return (X == other.X && Y == other.Y && Z == other.Z && IsSolid == other.IsSolid);
}
//--------------------------------------------------------------------------------------------------
bool Block::operator!=(const Block& other) const
{
	return !(*this == other);
}
/*----------------------------------------------------------------------------------------------------*/
void Block::LoadBlock(MeshData* meshData, WorldGeneration* world)
{
	MeshCreatorUtilities::FaceUp(meshData, LocalPosition);

	Block* north = world->GetBlock(X + 1, Y, Z);
	if (north == nullptr || !north->IsSolid)
	{
		MeshCreatorUtilities::FaceNorth(meshData, LocalPosition);
	}
	
	Block* south = world->GetBlock(X - 1, Y, Z);
	if (south == nullptr || !south->IsSolid)
	{
		MeshCreatorUtilities::FaceSouth(meshData, LocalPosition);
	}

	Block* west = world->GetBlock(X, Y - 1, Z);
	if (west == nullptr || !west->IsSolid)
	{
		MeshCreatorUtilities::FaceWest(meshData, LocalPosition);
	}

	Block* east = world->GetBlock(X, Y + 1, Z);
	if (east == nullptr || !east->IsSolid)
	{
		MeshCreatorUtilities::FaceEast(meshData, LocalPosition);
	}
}
//--------------------------------------------------------------------------------------------------
void Block::SetHCost(float hCost)
{
	if (_hCost == hCost)
	{
		return;
	}

	_hCost = hCost;
}
//--------------------------------------------------------------------------------------------------
void Block::SetGCost(float gCost)
{
	if (_gCost == gCost)
	{
		return;
	}

	_gCost = gCost;
}
//--------------------------------------------------------------------------------------------------
void Block::SetFCost(float fCost)
{
	if (_fCost == fCost)
	{
		return;
	}

	_fCost = fCost;
}
/*----------------------------------------------------------------------------------------------------*/
float Block::GetHCost() const
{
	return _hCost;
}
/*----------------------------------------------------------------------------------------------------*/
float Block::GetGCost() const
{
	return _gCost;
}
/*----------------------------------------------------------------------------------------------------*/
float Block::GetFCost() const
{
	return _hCost + _gCost;
}
/*----------------------------------------------------------------------------------------------------*/