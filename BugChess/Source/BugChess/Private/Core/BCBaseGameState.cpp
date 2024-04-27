// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Core/BCBaseGameState.h"

void ABCBaseGameState::BeginPlay()
{
	Super::BeginPlay();

	Board = NewObject<UGrid>();
	Board->InitGrid(8, 8, 150, FVector(0, 0, 100));
}
