// Fill out your copyright notice in the Description page of Project Settings.

#include "TileFactoryComponent.h"
#include "TileBase.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTileFactoryComponent::UTileFactoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ATileBase* UTileFactoryComponent::CreateTile(const FTransform& transform) const
{
	const auto numBlueprints = _availableTileBlueprints.Num();
	
	const auto indexToUse = FMath::RandRange(0, numBlueprints - 1);
	
	const auto& blueprintToUse = _availableTileBlueprints[indexToUse];
	
	return GetWorld()->SpawnActor<ATileBase>(blueprintToUse, transform);
}
