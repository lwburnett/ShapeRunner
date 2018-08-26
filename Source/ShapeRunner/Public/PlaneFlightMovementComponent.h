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
	void IntendToggleLeftInput(bool inputEnabled);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToggleRightInput(bool inputEnabled);

	void BeginPlay() override;
	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _propellerForceNewtons;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _rotationDegreesPerSecond;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _wingLiftCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _airDragCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _propellerForceAngleDegrees;

	bool _isPlaying;

	bool _isLeftInputEnabled;
	bool _isRightInputEnabled;

	UFUNCTION()
	void OnBeginPlaying();

	UFUNCTION()
	void OnBeginNotPlaying();

	static FVector GetHorizontalVector(const FRotator& rotator, float clampedThrow);

	bool InitializeGameStateSync();

	void ApplyWingLiftForce(AActor* owner, UPrimitiveComponent* body) const;

	void ApplyDragForce(AActor* owner, UPrimitiveComponent* body) const;

	void ApplyUserInput(AActor* owner, UPrimitiveComponent* body) const;

	void ApplyAcceleration(AActor* owner, UPrimitiveComponent* body) const;

	bool TryGetOwnerAndBody(AActor*& out_owner, UPrimitiveComponent*& out_body) const;

	bool ShouldApplyWingLift() const;
};
