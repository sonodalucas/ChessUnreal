// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/BoardComponent.h"
#include "Core/CustomLogging.h"
#include "Core/BCBaseGameMode.h"
#include "Core/GlobalSettings.h"
#include "Library/BCFunctionLibrary.h"
#include "Pieces/BCPiece.h"

DEFINE_LOG_CATEGORY(LogBoard);


// Sets default values for this component's properties
UBoardComponent::UBoardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UBoardComponent::StartBoard(FBoardInfo BoardInfo)
{
	for (int i = 0; i < 64; ++i)
	{
		if (EChessPiece::ECP_None == BoardInfo.PositionsArray[i].Piece)
			continue;
		
		GetPositionsOfPieceType(BoardInfo.PositionsArray[i].Piece) |= 1ull << i;
		GetPositionsOfPlayer(BoardInfo.PositionsArray[i].Colour) |= 1ull << i;
		
		SpawnPiece(BoardInfo.PositionsArray[i].Piece, BoardInfo.PositionsArray[i].Colour, i % 8, i / 8);
		
	}
	UE_LOGFMT(LogBoard, Warning, "Bit test: {0}", pawnsPositions & whitePiecesPositions);
}

void UBoardComponent::SpawnPiece(EChessPiece ChessPiece, EChessColour Colour, int PositionX, int PositionY) const
{
	UGlobalSettings* GlobalSettings = UBCFunctionLibrary::GetGlobalSettings();

	FText PieceText;
	UEnum::GetDisplayValueAsText(ChessPiece, PieceText);
	
	const auto PieceInfo = GlobalSettings->GetPiecesDataAsset()->FindRow<FPieceInfo>(FName(PieceText.ToString()), "UBoardComponent::CreatePiece");
	if (!PieceInfo)
	{
		UE_LOG(LogBoard, Error, TEXT("Piece [%s] not found in the pieces data table!"), *PieceText.ToString())
		return;
	}
	UE_LOG(LogBoard, Log, TEXT("Spawning piece [%s]."), *PieceText.ToString())
	const auto Piece = GetWorld()->SpawnActor<ABCPiece>(PieceClass, Grid->GetCellCenterWorldPosition(PositionX, PositionY), FRotator::ZeroRotator);
	Piece->SetupPiece(ChessPiece, Colour, PieceInfo->PieceMesh);
	UChessCellObject* CellObject = Cast<UChessCellObject>(Grid->GetGridObject(PositionX, PositionY));
	CellObject->SetPiece(Piece);
}

uint64& UBoardComponent::GetPositionsOfPieceType(EChessPiece PieceType)
{
	if (PieceType == ECP_Pawn)
	{
		return pawnsPositions;
	}
	if (PieceType == ECP_Knight)
	{
		return knightsPositions;
	}
	if (PieceType == ECP_Bishop)
	{
		return bishopsPositions;
	}
	if (PieceType == ECP_Rook)
	{
		return rooksPositions;
	}
	if (PieceType == ECP_Queen)
	{
		return queensPositions;
	}

	return kingsPositions;
}

uint64& UBoardComponent::GetPositionsOfPlayer(EChessColour PlayerColour)
{
	if (PlayerColour == ECC_White)
	{
		return whitePiecesPositions;
	}
	return blackPiecesPositions;
}

uint64 UBoardComponent::GetAllPiecesPosition() const
{
	return whitePiecesPositions || blackPiecesPositions;
}


// Called when the game starts
void UBoardComponent::BeginPlay()
{
	Super::BeginPlay();

	Grid = NewObject<UGrid>();
	Grid->InitGrid(8, 8, 150, FVector(0, 0, 30), CellClass);
}


