// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Core/BCBaseGameMode.h"
#include "Core/CustomLogging.h"
#include "Core/BCPlayerState.h"
#include "Kismet/GameplayStatics.h"

ABCBaseGameMode::ABCBaseGameMode()
{
	Board = CreateDefaultSubobject<UBoardComponent>(FName("Board"));
}

void ABCBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartGame();
}

void ABCBaseGameMode::Confirm(UChessCellObject* CellObject)
{
	if (PieceSelected)
		MovePiece(CellObject);
	else
		ChoosePiece(CellObject);
}

void ABCBaseGameMode::ChoosePiece(UChessCellObject* CellObject)
{
	if (!CellObject)
	{
		UE_LOGFMT(LogChessGameMode, Warning, "The cell given is null!");
		return;
	}

	// TODO: Get the legal moves for the given piece
}

void ABCBaseGameMode::MovePiece(UChessCellObject* DestinationCell)
{
	if (!DestinationCell)
	{
		UE_LOGFMT(LogChessGameMode, Warning, "The destination cell given is null!");
		return;
	}
	if (!PieceSelected)
	{
		UE_LOGFMT(LogChessGameMode, Warning, "There is no piece selected!");
		return;
	}
	
	EndTurn();
}

void ABCBaseGameMode::UnselectPiece()
{
	PieceSelected = nullptr;
}

void ABCBaseGameMode::StartGame()
{
	// It will be changed to white on the StartTurn method
	CurrentTurn = EChessColour::ECC_Black;

	// TODO: Uncomment this code when the AI is working, the AIC will be created around here.
	// const EChessColour PlayerColour = TEnumAsByte<EChessColour>(FMath::RandRange(0, 1));
	// Cast<ABCPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0))->SetPlayerColour(PlayerColour);

	StartTurn();
}

void ABCBaseGameMode::StartTurn()
{
	// Increment turn
	CurrentTurn = TEnumAsByte<EChessColour>((CurrentTurn + 1) % 2);
	OnTurnStarted.Broadcast(CurrentTurn);

	UE_LOGFMT(LogChessGameMode, Log, "Starting [{0}] player turn.", UEnum::GetDisplayValueAsText(CurrentTurn).ToString());
}

void ABCBaseGameMode::EndTurn()
{
	UE_LOGFMT(LogChessGameMode, Log, "Ending [{0}] player turn.", UEnum::GetDisplayValueAsText(CurrentTurn).ToString());
	OnTurnEnded.Broadcast(CurrentTurn);
}
