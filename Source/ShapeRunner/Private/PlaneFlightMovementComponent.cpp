// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaneFlightMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "RunnerGameState.h"
#include "Components/PrimitiveComponent.h"


UPlaneFlightMovementComponent::UPlaneFlightMovementComponent() :
		_propellerForceNewtons(40000.0),
		_rotationDegreesPerSecond(90),
		_wingLiftCoefficient(15),
		_airDragCoefficient(.001),
		_propellerForceAngleDegrees(10),
		_rollingForceMultiplier(.5), 
		_maxSpeed(2000),
		_isPlaying(false),
		_isLeftUpInputEnabled(false),
		_isLeftDownInputEnabled(false),
		_isRightUpInputEnabled(false),
		_isRightDownInputEnabled(false),
		_worldGravityAcceleration(0)
{
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

void UPlaneFlightMovementComponent::ApplyWingLiftForce(AActor* owner, UPrimitiveComponent* body) const
{
	auto direction = body->GetUpVector();

	body->AddForce(_wingLiftCoefficient * owner->GetVelocity().Size() * direction);
}

void UPlaneFlightMovementComponent::ApplyDragForce(AActor* owner, UPrimitiveComponent* body) const
{
	auto direction = body->GetForwardVector() * -1;

	body->AddForce(_airDragCoefficient * FMath::Square(owner->GetVelocity().Size()) * direction);
}

void UPlaneFlightMovementComponent::ApplyUserInput(AActor* owner, UPrimitiveComponent* body, float deltaTime) const
{
	const auto intendedMovementAction = DetermineIntendedMovementAction();

	switch (intendedMovementAction)
	{
		case Glide: 
			break;
		case AccelerateUp:
			ApplyUserAcceleration(owner, body, deltaTime, 1.0);
			break;
		case RollClockwise:
			ApplyUserRoll(owner, deltaTime, 1.0);
			break;
		case RollCounterClockwise:
			ApplyUserRoll(owner, deltaTime, -1.0);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Unknown EPlaneMovementAction %s"), intendedMovementAction);
			break;
	}
}

void UPlaneFlightMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InitializeGameStateSync())
		return;

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

	_worldGravityAcceleration = GetWorld()->GetGravityZ();
}

void UPlaneFlightMovementComponent::TickComponent(
	float deltaTime,
	ELevelTick tickType,
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);


	if (!_isPlaying)
		return;

	AActor* owner = nullptr;
	UPrimitiveComponent* body = nullptr;

	if (!TryGetOwnerAndBody(owner, body))
		return;

	if (!owner || !body)
		return;

	auto newRotation = owner->GetVelocity().Rotation();
	owner->SetActorRotation(FRotator(newRotation.Pitch, newRotation.Yaw, owner->GetActorRotation().Roll));

	ApplyUserInput(owner, body, deltaTime);

	// if (IsRolling())
	// 	ApplyStandardUpwardForce(owner, body);
	// else
	// 	ApplyWingLiftForce(owner, body);

	ApplyWingLiftForce(owner, body);

	ApplyDragForce(owner, body);
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

void UPlaneFlightMovementComponent::ApplyUserAcceleration(AActor* owner, UPrimitiveComponent* body, float deltaTime, double multiplier) const
{
	// Vertical force
	auto verticalForceMag = _propellerForceNewtons * FMath::Abs(FMath::Sin(_propellerForceAngleDegrees));
	auto upDirection = body->GetUpVector();

	auto verticalForce = multiplier * verticalForceMag * upDirection;

	body->AddForce(verticalForce);

	// Forward force
	auto speed = owner->GetVelocity().Size();
	if (_maxSpeed <= speed)
		return;

	auto potentialForwardForce = (_maxSpeed - speed) * body->GetMass() / deltaTime;

	auto forwardForceMag = FMath::Min(potentialForwardForce, _propellerForceNewtons * FMath::Abs(FMath::Cos(_propellerForceAngleDegrees)));
	auto forwardDirection = body->GetForwardVector();

	auto forwardForce = forwardForceMag * forwardDirection;

	body->AddForce(forwardForce);
}

void UPlaneFlightMovementComponent::ApplyUserRoll(AActor* owner, float deltaTime, double multiplier) const
{
	auto rollChange = multiplier * _rotationDegreesPerSecond * deltaTime;
	auto rotationChange = FRotator(0, 0, rollChange);

	owner->SetActorRotation(owner->GetActorRotation() + rotationChange);
}


// ReSharper disable CppParameterValueIsReassigned
bool UPlaneFlightMovementComponent::TryGetOwnerAndBody(AActor*& out_owner, UPrimitiveComponent*& out_body) const
// ReSharper restore CppParameterValueIsReassigned
{
	out_owner = GetOwner();

	if (!ensure(out_owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find owner."));
		return false;
	}

	out_body = Cast<UPrimitiveComponent>(out_owner->GetRootComponent());

	if (!ensure(out_body))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find body."));
		return false;
	}

	return true;
}

bool UPlaneFlightMovementComponent::IsRolling() const
{
	if (_isLeftUpInputEnabled && _isRightUpInputEnabled || !_isLeftUpInputEnabled && !_isRightUpInputEnabled)
		return false;

	return true;
}

UPlaneFlightMovementComponent::EPlaneMovementAction UPlaneFlightMovementComponent::DetermineIntendedMovementAction() const
{
	if (!_isLeftUpInputEnabled && !_isRightUpInputEnabled)
		return Glide;

	if (_isLeftUpInputEnabled && _isRightUpInputEnabled)
		return AccelerateUp;

	return _isRightUpInputEnabled ? RollClockwise : RollCounterClockwise;
}

void UPlaneFlightMovementComponent::IntendToggleLeftUpInput(const bool inputEnabled)
{
	_isLeftUpInputEnabled = inputEnabled;
}

void UPlaneFlightMovementComponent::IntendToggleLeftDownInput(const bool inputEnabled)
{
	_isLeftDownInputEnabled = inputEnabled;
}

void UPlaneFlightMovementComponent::IntendToggleRightUpInput(const bool inputEnabled)
{
	_isRightUpInputEnabled = inputEnabled;
}

void UPlaneFlightMovementComponent::IntendToggleRightDownInput(const bool inputEnabled)
{
	_isRightDownInputEnabled = inputEnabled;
}

void UPlaneFlightMovementComponent::ApplyStandardUpwardForce(AActor* actor, UPrimitiveComponent* body)
{
	if (FMath::Abs(_worldGravityAcceleration) < .001)
		_worldGravityAcceleration = GetWorld()->GetGravityZ();
	if (FMath::Abs(_worldGravityAcceleration) < .001)
		return;

	auto force = _worldGravityAcceleration * body->GetMass();
	auto direction = FVector(0.0, 0.0, 1.0);
	auto forceV = FMath::Abs(force) * direction;

	body->AddForce(forceV * _rollingForceMultiplier);
}
