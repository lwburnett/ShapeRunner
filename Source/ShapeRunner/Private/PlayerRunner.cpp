// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRunner.h"


// Sets default values
APlayerRunner::APlayerRunner()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerRunner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerRunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerRunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

