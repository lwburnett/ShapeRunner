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

	UFUNCTION(BlueprintCallable, Category = "Events")
	void HandleEndCrossed() const;

	FTileDelegate OnEndCrossed;

protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetNextTileTransform(const FTransform& transform);
	
private:
	FTransform _nextTileTransform;
};
