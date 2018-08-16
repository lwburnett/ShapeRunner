// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerPlane.generated.h"

/**
 * 
 */
UCLASS()
class SHAPERUNNER_API UPlayerPlane : public UStaticMeshComponent
{
	GENERATED_BODY()


public:

	UPlayerPlane();

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
