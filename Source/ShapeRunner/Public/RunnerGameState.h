// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Queue.h"
#include "GameFramework/GameStateBase.h"
#include "RunnerGameState.generated.h"

UENUM()
enum class EPlayState : uint8
{
	Loading,
	Playing,
	Pausing
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateDelegate);

class ATileBase;
class UTileFactoryComponent;

UCLASS()
class SHAPERUNNER_API ARunnerGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ARunnerGameState();

	bool IsPlaying() const;

	UFUNCTION(BlueprintCallable, Category = "")
	FRotator GetAbsoluteForward() const;

	FGameStateDelegate OnBeginPlaying;
	FGameStateDelegate OnBeginLoading;
	FGameStateDelegate OnBeginPausing;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int _maxNumTiles;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector _startPosition;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FRotator _absoluteForward;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ATileBase> _tileBlueprint;

	UTileFactoryComponent* _factory;

	TQueue<ATileBase*> _tileQueue;

	ATileBase* _mostRecentTile;

	EPlayState _playState;

	UFUNCTION()
	void HandleTileCrossed();

	void HandleTileCreation(const FTransform& transform, bool removeLast);

	void UpdatePlayState(EPlayState state);
};
