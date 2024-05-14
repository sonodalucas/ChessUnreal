// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Core/BCBaseGameState.h"

void ABCBaseGameState::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABCBaseGameState::Init(const int HalfMoves, const int FullMoves)
{
	HalfMoveCount = HalfMoves;
	FullMoveCount = FullMoves;
}

void ABCBaseGameState::IncrementFullMoveCount()
{
}

void ABCBaseGameState::IncrementHalfMoveCount()
{
}
