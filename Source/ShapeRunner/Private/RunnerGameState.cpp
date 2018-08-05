// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerGameState.h"
#include "Engine/World.h"
#include "TileBase.h"
#include "TileFactoryComponent.h"


ARunnerGameState::ARunnerGameState():
		_maxNumTiles(10),
		_startPosition(0.0),
		_tileBlueprint(nullptr), 
		_factory(nullptr),
		_mostRecentTile(nullptr)
{
}

void ARunnerGameState::BeginPlay()
{
	Super::BeginPlay();

	_factory = FindComponentByClass<UTileFactoryComponent>();

	auto rotation = FRotator(0.0);
	auto transform = FTransform(rotation, _startPosition);

	for (auto i=0; i < _maxNumTiles; i++)
	{
		HandleTileCreation(transform);
		transform = _mostRecentTile->GetNextTileTransform();
	}
}

void ARunnerGameState::HandleTileCrossed()
{
	HandleTileCreation(_mostRecentTile->GetNextTileTransform());
}

void ARunnerGameState::HandleTileCreation(const FTransform& transform)
{
	auto newTile = _factory->CreateTile(_tileBlueprint, transform);

	_tileQueue.push(newTile);

	if(_tileQueue.size() > _maxNumTiles)
	{
		auto tileToDestroy = _tileQueue.front();
		_tileQueue.pop();
		tileToDestroy->Destroy();
	}

	_mostRecentTile = newTile;

	_mostRecentTile->OnEndCrossed.AddUniqueDynamic(this, &ARunnerGameState::HandleTileCrossed);
}
