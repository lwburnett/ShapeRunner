// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PlaneWing.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class SHAPERUNNER_API UPlaneWing : public UStaticMeshComponent
{
	GENERATED_BODY()
	
		
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(FTransform wingTipTranform);

private:
	FTransform _wingTipTransform;
};
