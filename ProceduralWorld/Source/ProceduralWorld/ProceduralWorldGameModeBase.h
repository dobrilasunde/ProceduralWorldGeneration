// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProceduralWorldGameModeBase.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AGeneratedWorld;
class APathfinderMaster;
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

	UFUNCTION(BlueprintCallable)
	APathfinderMaster* GetPathfinderMaster() const;

public:
	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<AGeneratedWorld> _generatedWorldClass;
	UPROPERTY()
	AGeneratedWorld* _generatedWorld;

	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<APathfinderMaster> _pathfinderMasterClass;
	UPROPERTY()
	APathfinderMaster* _pathfinderMaster;
};
/*----------------------------------------------------------------------------------------------------*/
