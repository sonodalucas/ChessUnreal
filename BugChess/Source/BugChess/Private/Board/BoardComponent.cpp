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
	Grid = NewObject<UGrid>();
	Grid->InitGrid(8, 8, 150, FVector(0, 0, 30), CellClass);

	gameMode = Cast<ABCBaseGameMode>(GetWorld()->GetAuthGameMode());
	
	whiteCastleKingSide = BoardInfo.WhiteCastleKingSide;
	whiteCastleQueenSide = BoardInfo.WhiteCastleQueenSide;
	blackCastleKingSide = BoardInfo.BlackCastleKingSide;
	blackCastleQueenSide = BoardInfo.BlackCastleQueenSide;

	epFile = BoardInfo.EpFile;
	
	for (int i = 0; i < 64; ++i)
	{
		if (EChessPiece::ECP_None == BoardInfo.PositionsArray[i].Piece)
			continue;
		
		GetPieceTypeBitBoard(BoardInfo.PositionsArray[i].Piece, BoardInfo.PositionsArray[i].Colour) |= 1ull << i;
		
		SpawnPiece(BoardInfo.PositionsArray[i].Piece, BoardInfo.PositionsArray[i].Colour, i % 8, i / 8);
		
	}

	if (!showDebug) return;

	PrintBitboard();
	UE_LOGFMT(LogBoard, Warning, "Bit test: {0}", whitePawnsPositions);
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
	
	const auto Piece = GetWorld()->SpawnActor<ABCPiece>(PieceClass, Grid->GetCellCenterWorldPosition(PositionX, PositionY), FRotator::ZeroRotator);
	Piece->SetupPiece(ChessPiece, Colour, PieceInfo->PieceMesh);
	UChessCellObject* CellObject = Cast<UChessCellObject>(Grid->GetGridObject(PositionX, PositionY));
	CellObject->SetPiece(Piece);
}

uint64& UBoardComponent::GetPieceTypeBitBoard(EChessPiece PieceType, EChessColour PlayerColour)
{
	if (PieceType == ECP_Pawn)
	{
		return PlayerColour == ECC_White ? whitePawnsPositions : blackPawnsPositions;
	}
	if (PieceType == ECP_Knight)
	{
		return PlayerColour == ECC_White ? whiteKnightsPositions : blackKnightsPositions;
	}
	if (PieceType == ECP_Bishop)
	{
		return PlayerColour == ECC_White ? whiteBishopsPositions : blackBishopsPositions;
	}
	if (PieceType == ECP_Rook)
	{
		return PlayerColour == ECC_White ? whiteRooksPositions : blackRooksPositions;
	}
	if (PieceType == ECP_Queen)
	{
		return PlayerColour == ECC_White ? whiteQueensPositions : blackQueensPositions;
	}

	return PlayerColour == ECC_White ? whiteKingsPositions : blackKingsPositions;
}

uint64 UBoardComponent::GetPlayerBitBoard(EChessColour PlayerColour) const
{
	if (PlayerColour == ECC_White)
	{
		return whitePawnsPositions | whiteRooksPositions | whiteBishopsPositions | whiteKnightsPositions | whiteQueensPositions | whiteKingsPositions;
	}
	return blackPawnsPositions | blackRooksPositions | blackBishopsPositions | blackKnightsPositions | blackQueensPositions | blackKingsPositions;;
}

uint64 UBoardComponent::GetAllPiecesBitBoard() const
{
	return GetPlayerBitBoard(ECC_White) | GetPlayerBitBoard(ECC_Black);
}

void UBoardComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!GetWorld())
		return;
	if (GetWorld()->WorldType == EWorldType::Editor)
	{
		UE_LOGFMT(LogBoard, Error, "In editor");
		return;
	}
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UBoardComponent, ColourToDebug))
	{
		PrintBitboard();
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UBoardComponent, PieceToDebug))
	{
		PrintBitboard();
	}
	
}

void UBoardComponent::PrintBitboard()
{
	UE_LOGFMT(LogBoard, Log, "Printing current bit board for {0} {1}", *UEnum::GetDisplayValueAsText(ColourToDebug).ToString(), *UEnum::GetDisplayValueAsText(PieceToDebug).ToString());
	
	FString BinaryString = "";
	uint64 Binary = GetPieceTypeBitBoard(PieceToDebug, ColourToDebug);
	UE_LOGFMT(LogBoard, Log, "Current BitBoard representation is: {0}", Binary);

	while (Binary > 0)
	{
		BinaryString = FString::FromInt(Binary % 2) + BinaryString;
		Binary /= 2;
	}

	BinaryString.ReverseString();

	for (int y = 7; y >= 0; y--)
	{
		FString DebugString = "";
		for (int x = 7; x >= 0; x--)
		{
			
			if (BinaryString.IsValidIndex(8 * y + x))
				DebugString = FString("") + BinaryString[8 * y + x] + " " + DebugString;
			else
				DebugString = FString("0") + " " + DebugString;
		}
		UE_LOGFMT(LogBoard, Log, "{0}", DebugString );
	}
}

bool UBoardComponent::MovePiece_Implementation(UChessCellObject* OriginCell, UChessCellObject* DestinationCell)
{
	const FVector2d DestinationPosition = DestinationCell->GetPositionOnGrid();
	const FVector2d OriginPosition = OriginCell->GetPositionOnGrid();

	ABCPiece* PieceToCapture = DestinationCell->GetPiece();
	ABCPiece* PieceToMove = OriginCell->GetPiece();

	// TODO: Temporary. Remove this later
	if (IsValid(PieceToCapture) && PieceToCapture->GetColour() == PieceToMove->GetColour())
	{
		UE_LOGFMT(LogBoard, Warning, "Impossible to move to the selected cell, the cell {0} is occupied by the player piece.", *DestinationPosition.ToString());
		return false;
	}

	// Update BitBoards
	const uint64 PieceOriginBitBoard = 1ull << static_cast<int>(OriginPosition.Y * 8 + OriginPosition.X);
	const uint64 PieceDestinationBitBoard = 1ull << static_cast<int>(DestinationPosition.Y * 8 + DestinationPosition.X);

	UE_LOGFMT(LogBoard, Log, "Piece origin: {0} Piece Destination {1}", static_cast<int>(OriginPosition.Y * 8 + OriginPosition.X), static_cast<int>(DestinationPosition.Y * 8 + DestinationPosition.X));

	const uint64 PieceBitBoard = PieceOriginBitBoard | PieceDestinationBitBoard;
	GetPieceTypeBitBoard(PieceToMove->GetType(), PieceToMove->GetColour()) ^= PieceBitBoard;

	if (IsValid(PieceToCapture))
	{
		GetPieceTypeBitBoard(PieceToCapture->GetType(), PieceToCapture->GetColour()) ^= PieceDestinationBitBoard;
	}

	// Bit board debug
	if (showDebug)
	{
		PrintBitboard();
	}

	// Update cells
	OriginCell->SetPiece(nullptr);
	DestinationCell->SetPiece(PieceToMove);

	PieceToCapture->Destroy();
	
	PieceToMove->SetActorLocation(Grid->GetCellCenterWorldPosition(DestinationPosition.X, DestinationPosition.Y));
	
	return true;
}

// Called when the game starts
void UBoardComponent::BeginPlay()
{
	Super::BeginPlay();
}


