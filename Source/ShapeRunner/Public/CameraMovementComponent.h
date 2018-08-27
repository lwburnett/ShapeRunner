// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "CameraMovementComponent.generated.h"

class UCameraComponent;
/**
 * 
 */
UCLASS()
class SHAPERUNNER_API UCameraMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize(UCameraComponent* camera);


	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	UCameraComponent* _camera;
};
