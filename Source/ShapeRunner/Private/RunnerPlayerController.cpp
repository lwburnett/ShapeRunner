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
}

USphereMovementComponent* ARunnerPlayerController::GetMovementComponent(APawn* owner)
{
	return owner->FindComponentByClass<USphereMovementComponent>();
}
