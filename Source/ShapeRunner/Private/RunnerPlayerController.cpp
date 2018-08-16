// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerPlayerController.h"
#include "GameFramework/Pawn.h"
#include "SphereMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARunnerPlayerController::ARunnerPlayerController() :
		_movementComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARunnerPlayerController::BeginPlay()
{
	auto owner = GetPawn();

	if (!ensure(owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn found for player controller"));
		return;
	}

	_movementComponent = GetMovementComponent(owner);

	if (!ensure(_movementComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No movement component found for player controller"));
		return;
	}

	if (!HaveFloatingPawnMovementComponent(owner))
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("No floating pawn movement component found. %s may not move."),
			*GetNameSafe(owner));
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
