/*----------------------------------------------------------------------------------------------------*/
#include "Chunk.h"
#include "Block.h"
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk()
{

}
/*----------------------------------------------------------------------------------------------------*/
Chunk::Chunk(int x, int y, int z, int maxX, int maxY, int maxZ, TArray<TArray<TArray<Block*>>> grid): x(x), y(y), z(z), _maxX(maxX), _maxY(maxY), _maxZ(maxZ), _grid(grid)
{
	_chunkPosition = FVector(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(FVector worldPosition)
{
	int32 x = FMath::FloorToInt((worldPosition.X - _chunkPosition.X * 100.0f * _maxX) / 100.0f);
	int32 y = FMath::FloorToInt((worldPosition.Y - _chunkPosition.Y * 100.0f * _maxY) / 100.0f);
	int32 z = FMath::FloorToInt((worldPosition.Z - _chunkPosition.Z * 100.0f * _maxZ) / 100.0f);

	return GetBlock(x, y, z);
}
/*----------------------------------------------------------------------------------------------------*/
Block* Chunk::GetBlock(int32 x, int32 y, int32 z)
{
	if (x < 0 || y < 0 || z < 0 || x > _maxX || y > _maxY || z > _maxZ)
	{
		return nullptr;
	}

	return _grid[x][y][z];
}
//--------------------------------------------------------------------------------------------------
Block* Chunk::GetFirstSolidBlock()
{
	for (int x = 0; x < _maxX; ++x)
	{
		for (int y = 0; y < _maxY; ++y)
		{
			for (int z = 0; z < _maxZ; ++z)
			{
				if (_grid[x][y][z] != nullptr && _grid[x][y][z]->isSolid)
				{
					return _grid[x][y][z];
				}
			}
		}
	}

	return nullptr;
}
/*----------------------------------------------------------------------------------------------------*/