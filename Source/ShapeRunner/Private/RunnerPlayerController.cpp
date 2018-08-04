// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerPlayerController.h"
#include "GameFramework/Pawn.h"
#include "SphereMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARunnerPlayerController::ARunnerPlayerController() :
		_owner(nullptr),
		_movementComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARunnerPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	_movementComponent->IntendMoveForward();
}

void ARunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	_owner = GetPawn();

	if (!ensure(_owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn found for player controller"));
		return;
	}

	_movementComponent = GetMovementComponent(_owner);

	if (!ensure(_movementComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No movement component found for player controller"));
		return;
	}

	if (!HaveFloatingPawnMovementComponent(_owner))
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("No floating pawn movement component found. %s may not move."),
			*GetNameSafe(_owner));
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
