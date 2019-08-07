// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GodPlayerController.generated.h"
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API AGodPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGodPlayerController();

	void SetupInputComponent() override;
	
	void FindMousePosition();

	void CreateBox(FVector position);
};
/*----------------------------------------------------------------------------------------------------*/