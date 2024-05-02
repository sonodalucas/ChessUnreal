// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Core/BCBaseGameMode.h"

ABCBaseGameMode::ABCBaseGameMode()
{
	Board = CreateDefaultSubobject<UBoardComponent>(FName("Board"));
}

void ABCBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	Board->StartBoard();
}
