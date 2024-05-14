// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Core/BCBaseGameMode.h"

#include "Core/BCBaseGameState.h"
#include "Core/CustomLogging.h"
#include "Core/BCPlayerState.h"
#include "Library/BCFunctionLibrary.h"

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
	if (CellSelected)
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

	if (!IsValid(CellObject->GetPiece()))
	{
		UE_LOGFMT(LogChessGameMode, Warning, "The cell given is null!");
		return;
	}

	CellSelected = CellObject;
	
	// TODO: Get the legal moves for the given piece
}

void ABCBaseGameMode::MovePiece(UChessCellObject* DestinationCell)
{
	if (!DestinationCell)
	{
		UE_LOGFMT(LogChessGameMode, Warning, "The destination cell given is null!");
		return;
	}
	if (!CellSelected)
	{
		UE_LOGFMT(LogChessGameMode, Warning, "There is no cell selected!");
		return;
	}

	if (Board->MovePiece(CellSelected, DestinationCell))
	{
		CellSelected = nullptr;
		EndTurn();
	}
}

void ABCBaseGameMode::UnselectCell()
{
	CellSelected = nullptr;
}

void ABCBaseGameMode::StartGame()
{
	const FString Fen = FenString.IsEmpty() ? STARTING_LAYOUT : FenString;
	const auto BoardInfo = UBCFunctionLibrary::GetPositionsFromFen(Fen);
	Board->StartBoard(BoardInfo);


	GetGameState<ABCBaseGameState>()->Init(BoardInfo.HalfMoveCount, BoardInfo.FullMoveCount);

	// It will be changed to the correct turn in the StartTurn() method
	whiteToMove = !BoardInfo.WhiteToMove;

	// TODO: Uncomment this code when the AI is working, the AIC will be created around here.
	// const EChessColour PlayerColour = TEnumAsByte<EChessColour>(FMath::RandRange(0, 1));
	// Cast<ABCPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0))->SetPlayerColour(PlayerColour);

	StartTurn();
}

void ABCBaseGameMode::StartTurn()
{
	// Increment turn
	whiteToMove = !whiteToMove;
	OnTurnStarted.Broadcast(whiteToMove);

	UE_LOGFMT(LogChessGameMode, Log, "Starting [{0}] player turn.", whiteToMove ? *FString("White") : *FString("Black"));
}

void ABCBaseGameMode::EndTurn() const
{
	GetGameState<ABCBaseGameState>()->IncrementFullMoveCount();
	UE_LOGFMT(LogChessGameMode, Log, "Ending [{0}] player turn.", whiteToMove ? *FString("White") : *FString("Black"));
	OnTurnEnded.Broadcast(whiteToMove);
}
