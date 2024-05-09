// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/BCFunctionLibrary.h"
#include "Core/CustomLogging.h"
#include "Misc/Char.h"

UGlobalSettings* UBCFunctionLibrary::GetGlobalSettings()
{
	if (GEngine)
	{
		if (UGlobalSettings* DataInstance = Cast<UGlobalSettings>(GEngine->GameSingleton))
		{
			return DataInstance;
		}
	}

	return nullptr;

}

FBoardInfo UBCFunctionLibrary::GetPositionsFromFen(FString Fen)
{
	TArray<FString> Sections;
	Fen.ParseIntoArray(Sections, TEXT(" "), true);
	for (auto Section : Sections)
	{
		UE_LOGFMT(LogBCFunctionLibrary, Log, "FEN string segmented: {0}", *Section);
		
	}

	int x = 0;
	int y = 7;

	FBoardInfo BoardInfo;
	BoardInfo.PositionsArray.Init(FPositionInfo(), 64);
	
	for (const TCHAR Position : Sections[0])
	{
		if (Position == '/')
		{
			x = 0;
			y -= 1;
			continue;
		}

		if (TChar<WIDECHAR>::IsDigit(Position))
		{
			x += TChar<WIDECHAR>::ConvertCharDigitToInt(Position);
			continue;
		}

		const int CurrentPosition = 8 * y + x;
		BoardInfo.PositionsArray[CurrentPosition].Colour = TChar<WIDECHAR>::IsLower(Position) ? EChessColour::ECC_Black : EChessColour::ECC_White;
		EChessPiece PieceType;
		switch (TChar<WIDECHAR>::ToLower(Position))
		{
		case 'k':
			PieceType = EChessPiece::ECP_King;
			break;

		case 'q':
			PieceType = EChessPiece::ECP_Queen;
			break;

		case 'b':
			PieceType = EChessPiece::ECP_Bishop;
			break;

		case 'p':
			PieceType = EChessPiece::ECP_Pawn;
			break;

		case 'n':
			PieceType = EChessPiece::ECP_Knight;
			break;

		case 'r':
			PieceType = EChessPiece::ECP_Rook;
			break;
		default:
			PieceType = EChessPiece::ECP_None;
			;
		}
		
		BoardInfo.PositionsArray[CurrentPosition].Piece = PieceType;

		x += 1;
	}

	// Whose turn is it
	if (Sections.IsValidIndex(1))
	{
		BoardInfo.WhiteToMove = Sections[1][0] == 'w';
		UE_LOGFMT(LogBCFunctionLibrary, Log, "White turn ? {0}", BoardInfo.WhiteToMove);
	}

	if (Sections.IsValidIndex(2))
	{
		BoardInfo.WhiteCastleKingSide = Sections[2].Contains(FString("K"), ESearchCase::CaseSensitive);
		BoardInfo.WhiteCastleQueenSide = Sections[2].Contains(FString("Q"), ESearchCase::CaseSensitive);
		BoardInfo.BlackCastleKingSide = Sections[2].Contains(FString("k"), ESearchCase::CaseSensitive);
		BoardInfo.BlackCastleQueenSide = Sections[2].Contains(FString("q"), ESearchCase::CaseSensitive);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "White king side castle ? {0}", BoardInfo.WhiteCastleKingSide);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "White queen side castle ? {0}", BoardInfo.WhiteCastleQueenSide);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "Black king side castle ? {0}", BoardInfo.BlackCastleKingSide);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "Black queen side castle ? {0}", BoardInfo.BlackCastleQueenSide);
	}

	if (Sections.IsValidIndex(3))
	{
		FString Files = "abcdefgh";
		if (!Files.FindChar(Sections[3][0], BoardInfo.EpFile))
		{
			BoardInfo.EpFile = -1;
		}
		UE_LOGFMT(LogBCFunctionLibrary, Log, "En passant file index ? {0}", BoardInfo.EpFile);
	}

	if (Sections.IsValidIndex(4))
	{
		BoardInfo.HalfMoveCount = FCString::Atoi(*Sections[4]);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "Half move count ? {0}", BoardInfo.HalfMoveCount);
	}

	if (Sections.IsValidIndex(5))
	{
		BoardInfo.FullMoveCount = FCString::Atoi(*Sections[5]);
		UE_LOGFMT(LogBCFunctionLibrary, Log, "Full move count ? {0}", BoardInfo.FullMoveCount);
	}
	
	return BoardInfo;
}
