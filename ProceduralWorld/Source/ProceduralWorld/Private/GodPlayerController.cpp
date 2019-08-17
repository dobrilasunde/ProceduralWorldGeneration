// Fill out your copyright notice in the Description page of Project Settings.

#include "GodPlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "GeneratedWorld.h"
#include "Block.h"
/*----------------------------------------------------------------------------------------------------*/
AGodPlayerController::AGodPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TriggerLeftClick", IE_Pressed, this, &AGodPlayerController::FindMousePosition);
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::FindMousePosition()
{
	FHitResult hitResult;
	bool hit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	if (hit)
	{
		UE_LOG(LogTemp, Warning, TEXT("MouseHit: %f %f %f"), hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);
		AActor* hitActor = hitResult.GetActor();
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();

		Block* block = GetGeneratedWorld(this)->GetBlockFromWorldPosition(hitResult.ImpactPoint);
		if (block != nullptr)
		{
			CreateBox(block->worldPosition + FVector::UpVector * 100.0f / 2.0f);
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGodPlayerController::CreateBox(FVector position)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;
	AActor* emptyActor = GetWorld()->SpawnActor<AActor>(position, FRotator::ZeroRotator, spawnParams);

	UBoxComponent* Box = NewObject<UBoxComponent>(emptyActor);
	Box->SetWorldLocation(position);
	Box->bHiddenInGame = false;
	Box->RegisterComponentWithWorld(GetWorld());

	emptyActor->SetRootComponent(Box);
}
/*----------------------------------------------------------------------------------------------------*/


