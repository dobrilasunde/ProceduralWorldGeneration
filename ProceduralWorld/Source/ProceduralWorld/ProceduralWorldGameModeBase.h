// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProceduralWorldGameModeBase.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AGeneratedWorld;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class PROCEDURALWORLD_API AProceduralWorldGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProceduralWorldGameModeBase();
	
	virtual void InitGame(const FString& mapName, const FString& options, FString& errorMessage) override;

	UFUNCTION(BlueprintCallable)
	AGeneratedWorld* GetGeneratedWorld() const;

public:
	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<AGeneratedWorld> _generatedWorldClass;
	UPROPERTY()
	AGeneratedWorld* _generatedWorld;	
};
/*----------------------------------------------------------------------------------------------------*/
