// Fill out your copyright notice in the Description page of Project Settings.

#include "TileBase.h"


// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FTransform& ATileBase::GetNextTileTransform()
{
	return _nextTileTransform;
}

void ATileBase::SetNextTileTransform(const FTransform& transform)
{
	_nextTileTransform = FTransform(transform);
}

void ATileBase::HandleEndCrossed() const
{
	OnEndCrossed.Broadcast();
}
