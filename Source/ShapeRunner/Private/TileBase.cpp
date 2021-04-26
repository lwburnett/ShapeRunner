// Fill out your copyright notice in the Description page of Project Settings.

#include "TileBase.h"
#include "PlayerRunner.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlaneCollisionComponent.h"


// Sets default values
ATileBase::ATileBase() :
	_endCrossedEventAlreadyBroadcast(false),
	_endPlaneTag("EndPlane"),
	_attachPointTag("AttachPoint"),
	_collisionObjectsTag("CollisionObject")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

FTransform& ATileBase::GetNextTileTransform()
{
	return _nextTileTransform;
}

void ATileBase::Initialize()
{
	SetupAttachPoint();

	SetupEndPlane();

	SetupSceneComponentCollision();
}

void ATileBase::SetupAttachPoint()
{
	const auto attachPoints = GetComponentsByTag(UArrowComponent::StaticClass(), _attachPointTag);

	if (ensure(attachPoints.Num() == 1))
	{
		const auto attachPoint = Cast<UArrowComponent>(attachPoints[0]);

		_nextTileTransform = attachPoint->GetComponentTransform();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tiles must have exactly one instance of a %s with tag %s"), *UArrowComponent::StaticClass()->GetFName().ToString(), *_attachPointTag.ToString());
	}
}

void ATileBase::SetupEndPlane()
{
	const auto endPlanes = GetComponentsByTag(UBoxComponent::StaticClass(), _endPlaneTag);

	if (ensure(endPlanes.Num() == 1))
	{
		const auto endPlane = Cast<UBoxComponent>(endPlanes[0]);

		endPlane->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATileBase::OnEndPlaneBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tiles must have exactly one instance of a %s with tag %s"), *UBoxComponent::StaticClass()->GetFName().ToString(), *_endPlaneTag.ToString());
	}
}

void ATileBase::SetupSceneComponentCollision()
{
	const auto collisionObjects = GetComponentsByTag(UStaticMeshComponent::StaticClass(), _collisionObjectsTag);

	for (const auto collisionObject : collisionObjects)
	{
		const auto collisionObjectAsStaticMesh = Cast<UStaticMeshComponent>(collisionObject);
		collisionObjectAsStaticMesh->OnComponentHit.AddUniqueDynamic(this, &ATileBase::OnCollisionObjectHit);
	}
}

void ATileBase::OnEndPlaneBeginOverlap(
	UPrimitiveComponent* overlappedComponent,
	AActor* otherActor,
	UPrimitiveComponent* otherComponent,
	int otherBodyIndex,
	bool fromSweep,
	const FHitResult& sweepResult)
{
	FScopeLock ScopeLock(&_mutex);

	if (_endCrossedEventAlreadyBroadcast)
		return;
	
	const auto playerRunner = Cast<APlayerRunner>(otherActor);

	if (playerRunner)
	{
		_endCrossedEventAlreadyBroadcast = true;
		OnEndCrossed.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Something crossed the end plane that isn't the player."));
	}
}

void ATileBase::OnCollisionObjectHit(
	UPrimitiveComponent* hitComponent, 
	AActor* otherActor,
	UPrimitiveComponent* otherComponent, 
	FVector normalImpulse, 
	const FHitResult& hit)
{
	const auto playerRunner = Cast<APlayerRunner>(otherActor);

	if (playerRunner)
	{
		const auto playerCollisionComponent = playerRunner->FindComponentByClass(UPlaneCollisionComponent::StaticClass());

		if (ensure(playerCollisionComponent))
		{
			const auto playerCollisionComponentAsCollisionComponent = Cast<UPlaneCollisionComponent>(playerCollisionComponent);
			playerCollisionComponentAsCollisionComponent->OnDestructionCollision();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to find player collision component!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s collided with %s"), *otherComponent->GetFName().ToString(), *hitComponent->GetFName().ToString());
	}
}
