// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInteractions.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UPlayerInteractions::UPlayerInteractions()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlayerInteractions::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UPlayerInteractions::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UPlayerInteractions::FindMousePosition()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	const FVector start = playerController->GetFocalLocation();
	FRotator playerRot = playerController->GetControlRotation();
	const FVector end = start + playerRot.Vector() * 256;

	FHitResult HitData(ForceInit);
	UWorld* world = GetWorld();
	if (!world)
	{
		return;
	}

	FCollisionQueryParams traceParams(FName(TEXT("Terrain collision trace")), true, nullptr);
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;
	//traceParams.AddIgnoredActor(nullptr);

	HitData = FHitResult(ForceInit);
	world->LineTraceSingleByChannel(HitData, start, end, ECC_Pawn, traceParams);

	if (HitData.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: "), *(HitData.GetActor()->GetName()));
	}
}

