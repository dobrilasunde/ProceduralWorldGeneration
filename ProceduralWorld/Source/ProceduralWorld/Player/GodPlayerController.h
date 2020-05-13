// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GodPlayerController.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AGeneratedWorld;
class Block;
class AStaticMeshActor;
class AUnit;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API AGodPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGodPlayerController();

	virtual void Tick(float deltaTime) override;

	void SetupInputComponent() override;
	
	void FindMousePosition();

	void CreateBox(FVector position);

	void VisualizePosition();

	void RequestPath();

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AUnit> Unit;

private:
	UPROPERTY(EditAnywhere, Category = "Debugging")
	TSoftObjectPtr<AStaticMeshActor> _visualizer;
	
	FVector _mousePosition;
	
	Block* _currentBlock;
};
/*----------------------------------------------------------------------------------------------------*/