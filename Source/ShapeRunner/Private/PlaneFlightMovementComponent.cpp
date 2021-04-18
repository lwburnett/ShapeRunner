// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaneFlightMovementComponent.h"
#include "Engine/World.h"
#include "RunnerGameState.h"
#include "Components/PrimitiveComponent.h"


UPlaneFlightMovementComponent::UPlaneFlightMovementComponent() :
		_propellerForceNewtons(40000.0),
		_rotationDegreesPerSecond(90),
		_standardLiftExponent(.5),
		_airDragCoefficient(.001),
		_propellerForceAngleDegrees(30),
		_maxSpeed(2000),
		_glideHorizontalForceNewtons(5000.0),
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

void UPlaneFlightMovementComponent::ApplyDragForce(AActor* owner, UPrimitiveComponent* body) const
{
	auto direction = body->GetForwardVector() * -1;

	body->AddForce(_airDragCoefficient * FMath::Square(owner->GetVelocity().Size()) * direction);
}

void UPlaneFlightMovementComponent::ApplyGlideAcceleration(AActor* owner, UPrimitiveComponent* body, float deltaTime) const
{
	const auto rollDegrees = owner->GetActorRotation().Roll;
	const auto rollRadians = FMath::DegreesToRadians(rollDegrees);
	const auto sineRoll = FMath::Sin(rollRadians);

	const auto forward = owner->GetActorForwardVector();
	// little cheat to "project" forward vector onto x-y plane
	const auto projectedForward = FVector(forward.X, forward.Y, 0.0);

	const auto orthogonalForward = FVector(-projectedForward.Y, projectedForward.X, 0.0);
	const auto normalizedOrthogonalForward = orthogonalForward.GetSafeNormal();

	const auto force = normalizedOrthogonalForward * sineRoll * _glideHorizontalForceNewtons;
	body->AddForce(force);
}

void UPlaneFlightMovementComponent::ApplyUserInput(AActor* owner, UPrimitiveComponent* body, float deltaTime) const
{
	const auto intendedMovementAction = DetermineIntendedMovementAction();

	switch (intendedMovementAction)
	{
		case Glide:
			ApplyGlideAcceleration(owner, body, deltaTime);
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
		case AccelerateDown: 
			ApplyUserAcceleration(owner, body, deltaTime, -1.0);
			break;
		case AccelerateUpAndRollClockwise:
			ApplyUserAcceleration(owner, body, deltaTime, 0.25);
			ApplyUserRoll(owner, deltaTime, 0.5);
			break;
		case AccelerateUpAndRollCounterClockwise:
			ApplyUserAcceleration(owner, body, deltaTime, 0.25);
			ApplyUserRoll(owner, deltaTime, -0.5);
			break;
		case AccelerateDownAndRollClockwise:
			ApplyUserAcceleration(owner, body, deltaTime, -0.5);
			ApplyUserRoll(owner, deltaTime, 0.5);
			break;
		case AccelerateDownAndRollCounterClockwise:
			ApplyUserAcceleration(owner, body, deltaTime, -0.5);
			ApplyUserRoll(owner, deltaTime, -0.5);
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

	ApplyStandardUpwardForce(owner, body);

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
	const auto propellerForceAngleRadians = FMath::DegreesToRadians(_propellerForceAngleDegrees);
	auto verticalForceMag = _propellerForceNewtons * FMath::Abs(FMath::Sin(propellerForceAngleRadians));
	auto upDirection = body->GetUpVector();

	auto verticalForce = multiplier * verticalForceMag * upDirection;

	body->AddForce(verticalForce);

	// Forward force
	auto speed = owner->GetVelocity().Size();
	if (_maxSpeed <= speed)
		return;

	auto potentialForwardForce = (_maxSpeed - speed) * body->GetMass() / deltaTime;

	auto forwardForceMag = FMath::Min(potentialForwardForce, _propellerForceNewtons * FMath::Abs(FMath::Cos(propellerForceAngleRadians)));
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
	// TOption is UE4's c++ nullable. Just using this to avoid using another enum for now. Values are meant to mean:
	//    true == force up
	//    null == force neutral (no force)
	//    false == force down
	auto rightWingIntendedAction = TOptional<bool>();

	if (_isRightUpInputEnabled && !_isRightDownInputEnabled)
		rightWingIntendedAction = true;
	else if (!_isRightUpInputEnabled && _isRightDownInputEnabled)
		rightWingIntendedAction = false;
	
	TOptional<bool> leftWingIntendedAction{};
	
	if (_isLeftUpInputEnabled && !_isLeftDownInputEnabled)
		leftWingIntendedAction = true;
	else if (!_isLeftUpInputEnabled && _isLeftDownInputEnabled)
		leftWingIntendedAction = false;

	if (rightWingIntendedAction.IsSet())
	{
		if (rightWingIntendedAction.GetValue())
		{
			// right is up
			
			if (leftWingIntendedAction.IsSet())
			{
				if (leftWingIntendedAction.GetValue())
				{
					// left is up
					return AccelerateUp;
				}
				// left is down
				return RollCounterClockwise;
			}
			// left is neutral
			return AccelerateUpAndRollClockwise;
		}

		// right is down
		if (leftWingIntendedAction.IsSet())
		{
			if (leftWingIntendedAction.GetValue())
			{
				// left is up
				return RollClockwise;
			}

			// left is down
			return AccelerateDown;
		}

		//Left is neutral
		return AccelerateDownAndRollClockwise;
	}

	// right is neutral
	if (leftWingIntendedAction.IsSet())
	{
		if (leftWingIntendedAction.GetValue())
		{
			// left is up
			return AccelerateUpAndRollCounterClockwise;
		}

		// left is down
		return AccelerateDownAndRollCounterClockwise;
	}

	// left is neutral
	return Glide;
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

	const auto currentSpeed = actor->GetVelocity().Size();
	auto percentOfMaxSpeed = currentSpeed / _maxSpeed;

	if (!ensure(0.0 <= percentOfMaxSpeed && percentOfMaxSpeed <= 1.0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Percent max speed is %f. Should be between 0.0 and 1.0. Clamping"), percentOfMaxSpeed);
		percentOfMaxSpeed = FMath::Clamp(percentOfMaxSpeed, static_cast<float>(0.0), static_cast<float>(1.0));
	}
	
	const auto upwardForceMultiplier = FMath::Pow(percentOfMaxSpeed, _standardLiftExponent);
	const auto adjustedAcceleration = upwardForceMultiplier * _worldGravityAcceleration;
	const auto force = adjustedAcceleration * body->GetMass();
	const auto direction = FVector(0.0, 0.0, 1.0);
	const auto forceV = FMath::Abs(force) * direction;

	body->AddForce(forceV);
}
