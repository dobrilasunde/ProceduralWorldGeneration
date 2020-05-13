// Fill out your copyright notice in the Description page of Project Settings.

#include "GodPlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../World/GeneratedWorld.h"
#include "../Block/Block.h"
#include <DrawDebugHelpers.h>
#include "Engine/StaticMeshActor.h"
#include "../Pathfinding/PathfinderMaster.h"
#include "../Units/Unit.h"

/*----------------------------------------------------------------------------------------------------*/
AGodPlayerController::AGodPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}
//--------------------------------------------------------------------------------------------------
void AGodPlayerController::Tick(float deltaTime)
{
	FindMousePosition();
	VisualizePosition();
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TriggerLeftClick", IE_Pressed, this, &AGodPlayerController::RequestPath);
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::FindMousePosition()
{
	FHitResult hitResult;
	bool hit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	if (hit)
	{
		_mousePosition = hitResult.ImpactPoint;
		//DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 15.0f, 16, FColor::Yellow, false, 5.0f);
		//UE_LOG(LogTemp, Warning, TEXT("MouseHit: %f %f %f"), hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);
		
		Block* block = GetGeneratedWorld(this)->GetBlockFromWorldPosition(hitResult.ImpactPoint);
		if (block != nullptr)
		{
			_currentBlock = block;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::CreateBox(FVector position)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();
	AActor* emptyActor = GetWorld()->SpawnActor<AActor>(position, FRotator::ZeroRotator, spawnParams);

	UBoxComponent* Box = NewObject<UBoxComponent>(emptyActor);
	Box->SetWorldLocation(position);
	Box->bHiddenInGame = false;
	Box->RegisterComponentWithWorld(GetWorld());

	emptyActor->SetRootComponent(Box);
}
//--------------------------------------------------------------------------------------------------
void AGodPlayerController::VisualizePosition()
{
	if (_currentBlock != nullptr && _visualizer.IsValid())
	{
		FTransform newTransform =_visualizer->GetTransform();
		newTransform.SetTranslation(_currentBlock->WorldPosition + FVector::OneVector);
		_visualizer->SetActorTransform(newTransform);
	}
}
//--------------------------------------------------------------------------------------------------
void AGodPlayerController::RequestPath()
{
	if (_currentBlock == nullptr)
	{
		return;
	}

	APathfinderMaster* pathFinder = GetPathfinderMaster(this);
	if (pathFinder == nullptr)
	{
		return;
	}

	AGeneratedWorld* world = GetGeneratedWorld(this);
	if (world == nullptr)
	{
		return;
	}

	Block* firstBlock = world->GetBlockFromWorldPosition(Unit->GetActorTransform().GetLocation() - FVector::UpVector * 100.0f);
	if (firstBlock != nullptr)
	{
		pathFinder->RequestPathfind(firstBlock, _currentBlock, Unit.Get());
	}
}
/*----------------------------------------------------------------------------------------------------*/


