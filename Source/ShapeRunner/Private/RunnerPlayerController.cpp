// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerPlayerController.h"
#include "GameFramework/Pawn.h"
#include "SphereMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARunnerPlayerController::ARunnerPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARunnerPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto pawn = GetPawn();
	if(!ensure(pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn found for player controller"));
		return;
	}

	if(!HaveFloatingPawnMovementComponent(pawn))
		UE_LOG(
			LogTemp, 
			Warning, 
			TEXT("No floating pawn movement component found. %s may not move."), 
			*GetNameSafe(pawn));

	auto movementComponent = GetMovementComponent(pawn);

	if (!ensure(movementComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No movement component found for player controller"));
		return;
	}

		movementComponent->MoveForward();
}

USphereMovementComponent* ARunnerPlayerController::GetMovementComponent(APawn* owner)
{
	return owner->FindComponentByClass<USphereMovementComponent>();
}

bool ARunnerPlayerController::HaveFloatingPawnMovementComponent(APawn* owner)
{
	auto component = owner->FindComponentByClass<UFloatingPawnMovement>();

	return component != nullptr;
}
