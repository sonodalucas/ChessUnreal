// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/BoardComponent.h"

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

void UBoardComponent::StartBoard()
{
	for (int i = 0; i < 8; ++i)
	{
		SpawnPiece(EChessPiece::ECP_Pawn, EChessColour::ECC_Black, i, 6);
		SpawnPiece(EChessPiece::ECP_Pawn, EChessColour::ECC_White, i, 1);

		switch (i)
		{
		case 0:
		case 7:
			SpawnPiece(EChessPiece::ECP_Rook, EChessColour::ECC_Black, i, 7);
			SpawnPiece(EChessPiece::ECP_Rook, EChessColour::ECC_White, i, 0);
			break;

		case 1:
		case 6:
			SpawnPiece(EChessPiece::ECP_Knight, EChessColour::ECC_Black, i, 7);
			SpawnPiece(EChessPiece::ECP_Knight, EChessColour::ECC_White, i, 0);
			break;

		case 2:
		case 5:
			SpawnPiece(EChessPiece::ECP_Bishop, EChessColour::ECC_Black, i, 7);
			SpawnPiece(EChessPiece::ECP_Bishop, EChessColour::ECC_White, i, 0);
			break;

		case 3:
			SpawnPiece(EChessPiece::ECP_Queen, EChessColour::ECC_Black, i, 7);
			SpawnPiece(EChessPiece::ECP_Queen, EChessColour::ECC_White, i, 0);
			break;
			
		case 4:
			SpawnPiece(EChessPiece::ECP_King, EChessColour::ECC_Black, i, 7);
			SpawnPiece(EChessPiece::ECP_King, EChessColour::ECC_White, i, 0);
			break;
		default: ;
		}
	}
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


// Called when the game starts
void UBoardComponent::BeginPlay()
{
	Super::BeginPlay();

	Grid = NewObject<UGrid>();
	Grid->InitGrid(8, 8, 150, FVector(0, 0, 30), CellClass);
	StartBoard();
}


