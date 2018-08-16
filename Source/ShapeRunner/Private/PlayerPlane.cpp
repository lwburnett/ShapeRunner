// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPlane.h"


UPlayerPlane::UPlayerPlane()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerPlane::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto owner = GetOwner();

	if(!ensure(owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find owner."));
		return;
	}

	auto velocityUnit = owner->GetVelocity().Rotation();

	UE_LOG(LogTemp, Warning, TEXT("Velocity rotation: %s"), *velocityUnit.ToString());

	owner->SetActorRotation(velocityUnit);
}
