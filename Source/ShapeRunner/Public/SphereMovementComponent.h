// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "SphereMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHAPERUNNER_API USphereMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
	
	
public:
	USphereMovementComponent();

	void MoveForward() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _forwardMoveSpeed;
};
