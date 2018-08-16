// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerRunner.generated.h"


UCLASS()
class SHAPERUNNER_API APlayerRunner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerRunner();


protected:
	void BeginPlay() override;
};
