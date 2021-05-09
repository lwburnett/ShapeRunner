// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


void UCameraMovementComponent::Initialize(USpringArmComponent* cameraBoom, FRotator initialForward)
{
	_cameraBoom = cameraBoom;

	if (!ensure(_cameraBoom))
		UE_LOG(LogTemp, Warning, TEXT("Null camera boom given."));

	_desiredForward = initialForward;
	_cameraBoom->SetWorldRotation(_desiredForward);
}

void UCameraMovementComponent::TickComponent(
	float deltaTime, 
	ELevelTick tickType, 
	FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// const auto owner = GetOwner();
	//
	// if (!ensure(owner))
	// 	UE_LOG(LogTemp, Warning, TEXT("Cannot find owner!"));

	_cameraBoom->SetWorldRotation(_desiredForward);
}
