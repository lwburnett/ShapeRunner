// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerGameState.h"
#include "Engine/World.h"
#include "TileBase.h"
#include "TileFactoryComponent.h"


ARunnerGameState::ARunnerGameState():
		_maxNumTiles(10),
		_startPosition(0.0),
		_absoluteForward(0.0),
		_factory(nullptr),
		_mostRecentTile(nullptr), 
		_playState()
{
}

bool ARunnerGameState::IsPlaying() const
{
	return _playState == EPlayState::Playing;
}

FRotator ARunnerGameState::GetAbsoluteForward() const
{
	return _absoluteForward;
}

void ARunnerGameState::BeginLoad()
{
	UpdatePlayState(EPlayState::Loading);

	_factory = FindComponentByClass<UTileFactoryComponent>();

	auto rotation = FRotator(0.0);
	auto transform = FTransform(rotation, _startPosition);

	for (auto i = 0; i < _maxNumTiles; i++)
	{
		HandleTileCreation(transform, false);
		transform = _mostRecentTile->GetNextTileTransform();
	}

	UpdatePlayState(EPlayState::Playing);
}

void ARunnerGameState::HandleTileCrossed()
{
	HandleTileCreation(_mostRecentTile->GetNextTileTransform(), true);
}

void ARunnerGameState::HandleTileCreation(const FTransform& transform, bool removeLast)
{
	const auto newTile = _factory->CreateTile(transform);

	_tileQueue.Enqueue(newTile);

	if(removeLast)
	{
		ATileBase* tileToDestroy;
		if(_tileQueue.Dequeue(tileToDestroy))
			tileToDestroy->Destroy();
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to dequeue item from tile queue"));
	}

	_mostRecentTile = newTile;

	_mostRecentTile->OnEndCrossed.AddUniqueDynamic(this, &ARunnerGameState::HandleTileCrossed);
}

void ARunnerGameState::UpdatePlayState(EPlayState state)
{
	switch (state)
	{
	case EPlayState::Loading:
		{
			_playState = state;
			OnBeginLoading.Broadcast();
			break;
		}
	case EPlayState::Playing:
		{
			_playState = state;
			OnBeginPlaying.Broadcast();
			break;
		}
	case EPlayState::Pausing:
		{
			_playState = state;
			OnBeginPausing.Broadcast();
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("Unknown state encountered."));
			break;
		}
	}
}
