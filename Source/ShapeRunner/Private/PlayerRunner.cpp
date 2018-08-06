// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRunner.h"
#include "Engine/World.h"
#include "RunnerGameState.h"


// Sets default values
APlayerRunner::APlayerRunner()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerRunner::BeginPlay()
{
	Super::BeginPlay();

	auto gameState = GetWorld()->GetGameState<ARunnerGameState>();

	if(!gameState)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot get game state!"));
		return;
	}

	
}

