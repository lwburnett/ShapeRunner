// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCollisionComponent.h"
//#include "CameraMovementComponent.h"
#include "RunnerGameState.h"

// Sets default values for this component's properties
UPlaneCollisionComponent::UPlaneCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlaneCollisionComponent::OnDestructionCollision()
{
	const auto owner = GetOwner();

	// Detach camera
	//const auto cameraMovementComponent = owner->FindComponentByClass(TSubclassOf<UCameraMovementComponent>());

	// if (ensure(cameraMovementComponent))
	// {
	// 	owner->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Unable to find camera movement component."));
	// }

	const auto isDestroyed = owner->Destroy();

	if (!ensure(isDestroyed))
		UE_LOG(LogTemp, Error, TEXT("Failed to mark player pawn for destruction."));

	const auto world = GetWorld();

	if (ensure(world))
	{
		const auto gameState = world->GetGameState();

		if (ensure(gameState))
		{
			const auto runnerGameState = Cast<ARunnerGameState>(gameState);

			if (ensure(runnerGameState))
			{
				runnerGameState->BeginEndAfterPlayerDestruction();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to cast game state to runner game state."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get game state."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get game world."));
	}
}

