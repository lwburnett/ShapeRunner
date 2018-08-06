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

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float throwVal = 1.0) const;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveHorizontal(float throwVal) const;


	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _cruiseSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _horizontalMoveSpeed;

	bool _isPlaying;


	UFUNCTION()
	void OnBeginPlaying();

	UFUNCTION()
	void OnBeginNotPlaying();

	static FVector GetHorizontalVector(const FRotator& rotator, float clampedThrow);
};
