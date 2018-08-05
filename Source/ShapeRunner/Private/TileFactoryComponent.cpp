// Fill out your copyright notice in the Description page of Project Settings.

#include "TileFactoryComponent.h"
#include "TileBase.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTileFactoryComponent::UTileFactoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ATileBase* UTileFactoryComponent::CreateTile(UClass* blueprint, const FTransform& transform) const
{
	return GetWorld()->SpawnActor<ATileBase>(blueprint, transform);
}
