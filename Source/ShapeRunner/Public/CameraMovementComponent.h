// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "CameraMovementComponent.generated.h"

class USpringArmComponent;
/**
 * 
 */
UCLASS()
class SHAPERUNNER_API UCameraMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize(USpringArmComponent* cameraBoom);


	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	USpringArmComponent* _cameraBoom;
};
