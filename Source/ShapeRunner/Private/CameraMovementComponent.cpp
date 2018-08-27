// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"


void UCameraMovementComponent::Initialize(UCameraComponent* camera)
{
	_camera = camera;
}

void UCameraMovementComponent::TickComponent(
	float deltaTime, 
	ELevelTick tickType, 
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	auto ownerPosition = GetOwner()->GetActorLocation();

	auto xPosition = _camera->GetComponentLocation().X;
	auto newLocation = FVector(xPosition, ownerPosition.Y, ownerPosition.Z);
	_camera->SetWorldLocation(newLocation);
}
