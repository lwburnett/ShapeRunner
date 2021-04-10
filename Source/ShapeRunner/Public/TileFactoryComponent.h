// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileFactoryComponent.generated.h"


class ATileBase;


UCLASS(Blueprintable)
class SHAPERUNNER_API UTileFactoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTileFactoryComponent();

	ATileBase* CreateTile(const FTransform& transform) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<TSubclassOf<ATileBase>> _availableTileBlueprints;
};
