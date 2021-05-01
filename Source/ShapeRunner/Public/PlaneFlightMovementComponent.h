// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlaneFlightMovementComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHAPERUNNER_API UPlaneFlightMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
	
public:
	UPlaneFlightMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToggleLeftUpInput(bool inputEnabled);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToggleLeftDownInput(bool inputEnabled);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToggleRightUpInput(bool inputEnabled);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToggleRightDownInput(bool inputEnabled);

	void BeginPlay() override;
	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	enum EPlaneMovementAction
	{
		Glide,
		AccelerateUp,
		AccelerateDown,
		RollClockwise,
		RollCounterClockwise,
		AccelerateUpAndRollClockwise,
		AccelerateUpAndRollCounterClockwise,
		AccelerateDownAndRollClockwise,
		AccelerateDownAndRollCounterClockwise
	};
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _propellerForceNewtons;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _rotationDegreesPerSecond;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _standardLiftExponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _airDragCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _propellerForceAngleDegrees;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _maxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _glideHorizontalForceNewtons;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _movementCeilingHeight;

	bool _isPlaying;

	bool _isLeftUpInputEnabled;
	bool _isLeftDownInputEnabled;
	bool _isRightUpInputEnabled;
	bool _isRightDownInputEnabled;
	float _worldGravityAcceleration;

	UFUNCTION()
	void OnBeginPlaying();

	UFUNCTION()
	void OnBeginNotPlaying();

	static FVector GetHorizontalVector(const FRotator& rotator, float clampedThrow);

	bool InitializeGameStateSync();

	void ApplyDragForce(AActor* owner, UPrimitiveComponent* body) const;

	void ApplyGlideAcceleration(AActor* owner, UPrimitiveComponent* body, float deltaTime) const;
	
	void ApplyUserInput(AActor* owner, UPrimitiveComponent* body, float deltaTime) const;
	
	void ApplyUserRoll(AActor* actor, float deltaTime, double multiplier) const;

	void ApplyUserAcceleration(AActor* owner, UPrimitiveComponent* body, float deltaTime, double multiplier) const;

	void ApplyStandardUpwardForce(AActor* actor, UPrimitiveComponent* body);

	bool TryGetOwnerAndBody(AActor*& out_owner, UPrimitiveComponent*& out_body) const;

	EPlaneMovementAction DetermineIntendedMovementAction() const;
};
