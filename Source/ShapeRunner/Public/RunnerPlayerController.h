// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunnerPlayerController.generated.h"

class USphereMovementComponent;

UCLASS()
class SHAPERUNNER_API ARunnerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARunnerPlayerController();

	void Tick(float deltaTime) override;


protected:
	void BeginPlay() override;


private:
	APawn* _owner;
	USphereMovementComponent* _movementComponent;

	static USphereMovementComponent* GetMovementComponent(APawn* owner);

	static bool HaveFloatingPawnMovementComponent(APawn* owner);
};
