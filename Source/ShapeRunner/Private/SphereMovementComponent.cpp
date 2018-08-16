// Fill out your copyright notice in the Description page of Project Settings.

#include "SphereMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "RunnerGameState.h"


USphereMovementComponent::USphereMovementComponent() :
		_initialSpeed(10.0),
		_horizontalMoveSpeed(5.0),
		_isPlaying(false), 
		_lwing(nullptr), 
		_rwing(nullptr)

{
}

void USphereMovementComponent::IntendRotateClockwise(const float throwVal) const
{
	auto pawn = Cast<APawn>(GetOwner());

	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot move pawn horizontal"));
		return;
	}

	auto clampedThrow = FMath::Clamp<float>(throwVal, -1.0, 1.0);

	auto direction = GetHorizontalVector(pawn->GetActorRotation(), clampedThrow);

	pawn->AddMovementInput(direction, clampedThrow * _horizontalMoveSpeed);
}

void USphereMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	auto gameState = GetWorld()->GetGameState<ARunnerGameState>();

	if(!ensure(gameState))
	{
		UE_LOG(LogTemp, Error, TEXT("No game state found."));
		return;
	}

	gameState->OnBeginPlaying.AddUniqueDynamic(this, &USphereMovementComponent::OnBeginPlaying);
	gameState->OnBeginLoading.AddUniqueDynamic(this, &USphereMovementComponent::OnBeginNotPlaying);
	gameState->OnBeginPausing.AddUniqueDynamic(this, &USphereMovementComponent::OnBeginNotPlaying);

	_isPlaying = gameState->IsPlaying();

	auto owner = Cast<APawn>(GetOwner());

	if (!ensure(owner))
	{
		UE_LOG(LogTemp, Error, TEXT("No owner found."));
		return;
	}

	auto forwardVector = owner->GetActorRotation().Vector();
}

void USphereMovementComponent::TickComponent(
	float deltaTime, 
	ELevelTick tickType, 
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (!_isPlaying)
		return;

	auto pawn = Cast<APawn>(GetOwner());

	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot move pawn forward"));
		return;
	}

	auto fowardVector = pawn->GetActorRotation().Vector();

	pawn->AddMovementInput(fowardVector, _initialSpeed);
}

FVector USphereMovementComponent::GetHorizontalVector(const FRotator& rotator, const float clampedThrow)
{
	auto toAdd = clampedThrow > 0.0 ? 90.0 : -90.0;

	auto newYaw = rotator.Yaw + toAdd;

	if (newYaw > 360.0)
		newYaw -= 360;

	else if (newYaw < 0.0)
		newYaw = 360 - newYaw;

	auto ret = rotator;
	ret.Yaw = newYaw;

	return ret.Vector();
}

void USphereMovementComponent::OnBeginPlaying()
{
	_isPlaying = true;
}

void USphereMovementComponent::OnBeginNotPlaying()
{
	_isPlaying = false;
}

void USphereMovementComponent::Initialize(UPlaneWing* lwing, UPlaneWing* rwing)
{
	_lwing = lwing;
	_rwing = rwing;
}

void USphereMovementComponent::IntendAccelerate(float throwVal) const
{
}
