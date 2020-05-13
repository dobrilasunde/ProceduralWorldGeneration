/*----------------------------------------------------------------------------------------------------*/
#include "Chunk.h"
#include "../Block/Block.h"
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk()
{

}
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk(int x, int y, int z, int maxX, int maxY, int maxZ, TArray<TArray<TArray<Block*>>> grid): X(x), Y(y), Z(z), MaxX(maxX), MaxY(maxY), MaxZ(maxZ), _grid(grid)
{
	_chunkPosition = FVector(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(FVector worldPosition)
{
	int32 x = FMath::FloorToInt((worldPosition.X - _chunkPosition.X * 100.0f * MaxX) / 100.0f);
	int32 y = FMath::FloorToInt((worldPosition.Y - _chunkPosition.Y * 100.0f * MaxY) / 100.0f);
	int32 z = FMath::FloorToInt((worldPosition.Z - _chunkPosition.Z * 100.0f * MaxZ) / 100.0f);

	return GetBlock(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > MaxX || y > MaxY || z > MaxZ)
	{
		return nullptr;
	}

	return _grid[x][y][z];
}
//--------------------------------------------------------------------------------------------------
Block* Chunk::GetFirstSolidBlock()
{
	for (int x = 0; x < MaxX; ++x)
	{
		for (int y = 0; y < MaxY; ++y)
		{
			for (int z = 0; z < MaxZ; ++z)
			{
				if (_grid[x][y][z] != nullptr && _grid[x][y][z]->IsSolid)
				{
					return _grid[x][y][z];
				}
			}
		}
	}

	return nullptr;
}
/*----------------------------------------------------------------------------------------------------*/