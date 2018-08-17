// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlaneFlightMovementComponent.generated.h"

class UArrowComponent;
class UPlaneWing;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHAPERUNNER_API UPlaneFlightMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
	
public:
	UPlaneFlightMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendAccelerate(float throwVal = 1) const;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendRotateClockwise(float throwVal) const;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UPlaneWing* lwing, UPlaneWing* rwing, UArrowComponent* propeller);

	void BeginPlay() override;

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _propellerForceNewtons;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _horizontalMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _wingLiftCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float _airDragCoefficient;

	bool _isPlaying;

	UPlaneWing* _lwing;
	UPlaneWing* _rwing;
	UArrowComponent* _propeller;


	UFUNCTION()
	void OnBeginPlaying();

	UFUNCTION()
	void OnBeginNotPlaying();

	static FVector GetHorizontalVector(const FRotator& rotator, float clampedThrow);

	bool InitializeGameStateSync();

	void ApplyWingLiftForce(AActor* body);

	void ApplyDragForce(AActor* body);
};
