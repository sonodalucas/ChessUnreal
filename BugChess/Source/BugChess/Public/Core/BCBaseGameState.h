// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BugChess/Public/Board/Grid.h"
#include "GameFramework/GameState.h"
#include "BCBaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class BUGCHESS_API ABCBaseGameState : public AGameState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UGrid* Board;
};
