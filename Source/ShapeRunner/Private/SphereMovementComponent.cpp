// Fill out your copyright notice in the Description page of Project Settings.

#include "SphereMovementComponent.h"
#include "GameFramework/Pawn.h"


void USphereMovementComponent::MoveForward() const
{
	auto pawn = Cast<APawn>(GetOwner());

	if(!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot move pawn forward"));
		return;
	}

	auto fowardVector = pawn->GetActorRotation().Vector();

	pawn->AddMovementInput(fowardVector, _forwardMoveSpeed);
}

USphereMovementComponent::USphereMovementComponent(): 
		_forwardMoveSpeed(10.0)
{
}
