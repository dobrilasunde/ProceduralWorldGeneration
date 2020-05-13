// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit.h"
#include "../Block/Block.h"

//--------------------------------------------------------------------------------------------------
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Unit"));
	_mesh->SetupAttachment(RootComponent);
}
//--------------------------------------------------------------------------------------------------
void AUnit::Tick(float deltaTime)
{
	if (_pathIndex > _path.Num() - 1)
	{
		_hasPath = false;
		_lerpAlpha = 0.0f;
		return;
	}

	if (!_initLerp)
	{
		_lerpAlpha = _lerpAlpha - 1.0f;
		_lerpAlpha = FMath::Clamp(_lerpAlpha, 0.0f, 1.0f);
		_startPosition = GetActorLocation();
		_endPosition = _path[_pathIndex]->WorldPosition + FVector::UpVector * 100.0f;

		float distance = FVector::Distance(_startPosition, _endPosition);
		_actualSpeed = _speed / distance;
		_initLerp = true;
	}
	
	_lerpAlpha += deltaTime * _actualSpeed;
	if (_lerpAlpha > 1.0f)
	{
		_pathIndex++;
		_initLerp = false;
	}

	FVector targetPosition = FMath::Lerp(_startPosition, _endPosition, _lerpAlpha);
	SetActorLocation(targetPosition);
}
//--------------------------------------------------------------------------------------------------
void AUnit::LoadPath(TArray<Block*>& path)
{
	_path = path;
	_hasPath = true;
	_pathIndex = 0;
}
//--------------------------------------------------------------------------------------------------