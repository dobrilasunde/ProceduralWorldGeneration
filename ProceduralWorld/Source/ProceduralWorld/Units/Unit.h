// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class Block;
class UStaticMeshComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API AUnit : public AActor
{
	GENERATED_BODY()

public:
	AUnit();

	virtual void Tick(float deltaTime) override;

	void LoadPath(TArray<Block*>& path);


private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _mesh;

	TArray<Block*> _path;
	int32 _pathIndex;
	bool _hasPath;

	bool _initLerp;
	float _lerpAlpha;
	UPROPERTY(EditAnywhere)
	float _speed = 3.0f;
	float _actualSpeed;

	FVector _startPosition;
	FVector _endPosition;
};
/*----------------------------------------------------------------------------------------------------*/