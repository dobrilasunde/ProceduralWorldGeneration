/*----------------------------------------------------------------------------------------------------*/
#include "Chunk.h"
#include "Block.h"
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk()
{

}
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk(int x, int y, int z, int maxX, int maxY, int maxZ, TArray<TArray<TArray<Block*>>> grid): x(x), y(y), z(z), maxX(maxX), maxY(maxY), maxZ(maxZ), grid(grid)
{
	chunkPosition = FVector(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(FVector worldPosition)
{
	int32 x = FMath::FloorToInt((worldPosition.X - chunkPosition.X * 100.0f * maxX) / 100.0f);
	int32 y = FMath::FloorToInt((worldPosition.Y - chunkPosition.Y * 100.0f * maxY) / 100.0f);
	int32 z = FMath::FloorToInt((worldPosition.Z - chunkPosition.Z * 100.0f * maxZ) / 100.0f);

	return GetBlock(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > maxX - 1 || y > maxY - 1 || z > maxZ - 1)
	{
		return nullptr;
	}

	return grid[x][y][z];
}
/*----------------------------------------------------------------------------------------------------*/