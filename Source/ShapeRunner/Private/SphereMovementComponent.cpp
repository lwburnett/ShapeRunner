// Fill out your copyright notice in the Description page of Project Settings.

#include "SphereMovementComponent.h"
#include "GameFramework/Pawn.h"


USphereMovementComponent::USphereMovementComponent() :
		_cruiseSpeed(10.0),
		_horizontalMoveSpeed(5.0)
{
}

void USphereMovementComponent::IntendMoveForward(const float throwVal) const
{
	auto pawn = Cast<APawn>(GetOwner());

	if(!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot move pawn forward"));
		return;
	}

	auto clampedThrow = FMath::Clamp<float>(throwVal, -1.0, 1.0);

	auto fowardVector = pawn->GetActorRotation().Vector();

	pawn->AddMovementInput(fowardVector, clampedThrow * _cruiseSpeed);
}

void USphereMovementComponent::IntendMoveHorizontal(const float throwVal) const
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
