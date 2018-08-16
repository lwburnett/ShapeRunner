// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "SphereMovementComponent.generated.h"


class UPlaneWing;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHAPERUNNER_API USphereMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
	
	
public:
	USphereMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendAccelerate(float throwVal = 1) const;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendRotateClockwise(float throwVal) const;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UPlaneWing* lwing, UPlaneWing* rwing);

	void BeginPlay() override;

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _initialSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _horizontalMoveSpeed;

	bool _isPlaying;

	UPlaneWing* _lwing;
	UPlaneWing* _rwing;


	UFUNCTION()
	void OnBeginPlaying();

	UFUNCTION()
	void OnBeginNotPlaying();

	static FVector GetHorizontalVector(const FRotator& rotator, float clampedThrow);
};
