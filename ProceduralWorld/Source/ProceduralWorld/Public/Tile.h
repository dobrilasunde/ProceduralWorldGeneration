// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROCEDURALWORLD_API Tile
{
public:
	Tile(int32 x, int32 y);
	~Tile();

public:
	int32 mX;
	int32 mY;
};
