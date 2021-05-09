// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"


void UCameraMovementComponent::Initialize(USpringArmComponent* cameraBoom)
{
	_cameraBoom = cameraBoom;
}

void UCameraMovementComponent::TickComponent(
	float deltaTime, 
	ELevelTick tickType, 
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// auto ownerPosition = GetOwner()->GetActorLocation();
	//
	// auto xPosition = _cameraBoom->GetComponentLocation().X;
	// auto newLocation = FVector(xPosition, ownerPosition.Y, ownerPosition.Z);
	// _cameraBoom->SetWorldLocation(newLocation);
}
