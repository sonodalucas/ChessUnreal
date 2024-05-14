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

public:
	void Init(int HalfMoves, int FullMoves);

	void IncrementFullMoveCount();

	void IncrementHalfMoveCount();

protected:
	// Number of half-moves since last capture or pawn advance
	// (starts at 0 and increments after each player's move)
	UPROPERTY(BlueprintReadOnly)
	int HalfMoveCount = 0;
	
	// Total number of moves played in the game
	// (starts at 1 and increments after black's move)
	UPROPERTY(BlueprintReadOnly)
	int FullMoveCount = 0;
};
