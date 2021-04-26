// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTileDelegate);

UCLASS()
class SHAPERUNNER_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATileBase();

	FTransform& GetNextTileTransform();

	FTileDelegate OnEndCrossed;
	
	void Initialize();
	
private:
	FTransform _nextTileTransform;
	FCriticalSection _mutex;
	bool _endCrossedEventAlreadyBroadcast;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FName _endPlaneTag;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FName _attachPointTag;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FName _collisionObjectsTag;
	
	void SetupAttachPoint();
	
	void SetupEndPlane();
	
	void SetupSceneComponentCollision();

	UFUNCTION()
	void OnEndPlaneBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnCollisionObjectHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit);
};
