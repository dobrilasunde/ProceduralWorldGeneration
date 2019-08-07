// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshCreatorUtilities.h"
#include "MeshData.h"
#include "Tile.h"

const float MeshCreatorUtilities::mTileScale = 0.25f;

/*static*/
void MeshCreatorUtilities::FaceUp(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z + 50));
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z + 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData, Direction::Up);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::FaceDown(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z - 50));
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z - 50));
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z - 50));
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z - 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::FaceNorth(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z - 50));
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z + 50));
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z - 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::FaceEast(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z - 50));
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X + 50, origin.Y + 50, origin.Z - 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::FaceWest(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z - 50));
	vertices.Add(FVector(origin.X + 50, origin.Y - 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z - 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::FaceSouth(MeshData* meshData, FVector origin)
{
	TArray<FVector> vertices;
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z - 50));
	vertices.Add(FVector(origin.X - 50, origin.Y - 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z + 50));
	vertices.Add(FVector(origin.X - 50, origin.Y + 50, origin.Z - 50));

	meshData->mVertices.Append(vertices);

	AddUVs(meshData);
	AddTriangles(meshData);
}

/*static*/
void MeshCreatorUtilities::AddTriangles(MeshData* meshData)
{
	TArray<int32> triangles;
	triangles.Add(meshData->mVertices.Num() - 2);
	triangles.Add(meshData->mVertices.Num() - 3);
	triangles.Add(meshData->mVertices.Num() - 4);

	triangles.Add(meshData->mVertices.Num() - 1);
	triangles.Add(meshData->mVertices.Num() - 2);
	triangles.Add(meshData->mVertices.Num() - 4);

	meshData->mTriangles.Append(triangles);
}

/*static*/
void MeshCreatorUtilities::AddUVs(MeshData* meshData, Direction direction)
{
	TArray<FVector2D> uv;
	Tile* tilePosition = TexturePosition(direction);

	//uv.Add(FVector2D(0, 0));
	//uv.Add(FVector2D(0, 1));
	//uv.Add(FVector2D(1, 1));
	//uv.Add(FVector2D(1, 0));

	uv.Add(FVector2D(mTileScale * tilePosition->mX, mTileScale * tilePosition->mY + mTileScale));
	uv.Add(FVector2D(mTileScale * tilePosition->mX, mTileScale * tilePosition->mY));
	uv.Add(FVector2D(mTileScale * tilePosition->mX + mTileScale, mTileScale * tilePosition->mY));
	uv.Add(FVector2D(mTileScale * tilePosition->mX + mTileScale, mTileScale * tilePosition->mY + mTileScale));

	meshData->mUV.Append(uv);
}

/*static*/
Tile* MeshCreatorUtilities::TexturePosition(Direction direction)
{
	switch (direction)
	{
	case Direction::Up:
		return new Tile(2, 3);
	default:
	case Direction::Other:
		return new Tile(3, 3);
	}
}
