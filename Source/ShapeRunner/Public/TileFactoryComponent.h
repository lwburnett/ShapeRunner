// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileFactoryComponent.generated.h"


class ATileBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHAPERUNNER_API UTileFactoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTileFactoryComponent();

	ATileBase* CreateTile(UClass* blueprint, const FTransform& transform) const;
};
