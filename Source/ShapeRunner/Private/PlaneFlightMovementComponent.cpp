// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaneFlightMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "RunnerGameState.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ArrowComponent.h"


UPlaneFlightMovementComponent::UPlaneFlightMovementComponent() :
		_propellerForceNewtons(50000.0),
		_horizontalMoveSpeed(5.0),
		_wingLiftCoefficient(20),
		_airDragCoefficient(.001), 
		_propellerForceAngleDegrees(20),
		_isPlaying(false),
		_lwing(nullptr),
		_rwing(nullptr),
		_propeller(nullptr)
{
}

void UPlaneFlightMovementComponent::IntendRotateClockwise(const float throwVal) const
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

bool UPlaneFlightMovementComponent::InitializeGameStateSync()
{
	auto gameState = GetWorld()->GetGameState<ARunnerGameState>();

	if (!ensure(gameState))
	{
		UE_LOG(LogTemp, Error, TEXT("No game state found."));
		return true;
	}

	gameState->OnBeginPlaying.AddUniqueDynamic(this, &UPlaneFlightMovementComponent::OnBeginPlaying);
	gameState->OnBeginLoading.AddUniqueDynamic(this, &UPlaneFlightMovementComponent::OnBeginNotPlaying);
	gameState->OnBeginPausing.AddUniqueDynamic(this, &UPlaneFlightMovementComponent::OnBeginNotPlaying);

	_isPlaying = gameState->IsPlaying();
	return false;
}

void UPlaneFlightMovementComponent::ApplyWingLiftForce(AActor* owner)
{
	auto body = Cast<UPrimitiveComponent>(owner->GetRootComponent());

	if(!ensure(body))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find body"));
		return;
	}

	auto direction = body->GetUpVector();

	body->AddForce(_wingLiftCoefficient * owner->GetVelocity().Size() * direction);
}

void UPlaneFlightMovementComponent::ApplyDragForce(AActor* owner)
{
	auto body = Cast<UPrimitiveComponent>(owner->GetRootComponent());

	if (!ensure(body))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find body"));
		return;
	}

	auto direction = body->GetForwardVector() * -1;

	body->AddForce(_airDragCoefficient * FMath::Square(owner->GetVelocity().Size()) * direction);
}

void UPlaneFlightMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InitializeGameStateSync())
		return;
}

void UPlaneFlightMovementComponent::TickComponent(
	float deltaTime,
	ELevelTick tickType,
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);


	if (!_isPlaying)
		return;

	auto owner = GetOwner();

	if (!ensure(owner))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot move pawn forward"));
		return;
	}

	owner->SetActorRotation(owner->GetVelocity().Rotation());

	ApplyWingLiftForce(owner);

	ApplyDragForce(owner);
}

FVector UPlaneFlightMovementComponent::GetHorizontalVector(const FRotator& rotator, const float clampedThrow)
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

void UPlaneFlightMovementComponent::OnBeginPlaying()
{
	_isPlaying = true;
}

void UPlaneFlightMovementComponent::OnBeginNotPlaying()
{
	_isPlaying = false;
}

void UPlaneFlightMovementComponent::Initialize(UPlaneWing* lwing, UPlaneWing* rwing, UArrowComponent* propeller)
{
	_lwing = lwing;
	_rwing = rwing;
	_propeller = propeller;
}

void UPlaneFlightMovementComponent::IntendAccelerate(float throwVal) const
{
	auto owner = GetOwner();

	if (!ensure(owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find owner."));
		return;
	}

	auto body = Cast<UPrimitiveComponent>(owner->GetRootComponent());

	if (!ensure(body))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find body."));
		return;
	}

	auto rotation = FRotator(_propellerForceAngleDegrees, 0, 0);
	auto forward = body->GetForwardVector();
	auto direction = rotation.RotateVector(forward);

	auto force = _propellerForceNewtons * throwVal * direction;

	body->AddForce(force);
}
