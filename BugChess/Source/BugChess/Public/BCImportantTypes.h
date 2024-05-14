// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BCImportantTypes.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum EChessColour
{
	ECC_None = 0		UMETA(DisplayName="None", Hidden),
	ECC_White = 1		UMETA(DisplayName="White"),
	ECC_Black = 2		UMETA(DisplayName="Black")
};

UENUM(BlueprintType)
enum EChessPiece
{
	ECP_None = 0		UMETA(DisplayName="None", Hidden),
	ECP_Pawn = 1		UMETA(DisplayName="Pawn"),
	ECP_Knight = 2		UMETA(DisplayName="Knight"),
	ECP_Bishop = 3		UMETA(DisplayName="Bishop"),
	ECP_Rook = 4		UMETA(DisplayName="Rook"),
	ECP_Queen = 5		UMETA(DisplayName="Queen"),
	ECP_King = 6		UMETA(DisplayName="King")
};

UCLASS()
class BUGCHESS_API UBCImportantTypes : public UObject
{
	GENERATED_BODY()
};

USTRUCT()
struct FPieceInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UStaticMesh* PieceMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FPositionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EChessColour> Colour;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EChessPiece> Piece;

	FPositionInfo()
	{
		Colour = ECC_None;
		Piece = EChessPiece::ECP_None;
	}
};

USTRUCT(BlueprintType)
struct FBoardInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FPositionInfo> PositionsArray;

	// Castling
	UPROPERTY(BlueprintReadOnly)
	bool WhiteCastleKingSide = true;
	
	UPROPERTY(BlueprintReadOnly)
	bool WhiteCastleQueenSide = true;
	
	UPROPERTY(BlueprintReadOnly)
	bool BlackCastleKingSide = true;
	
	UPROPERTY(BlueprintReadOnly)
	bool BlackCastleQueenSide = true;
	
	// En passant file (0 is a-file, 7 is h-file, -1 means none)
	UPROPERTY(BlueprintReadOnly)
	int EpFile = -1;
	
	UPROPERTY(BlueprintReadOnly)
	bool WhiteToMove = true;
	
	// Number of half-moves since last capture or pawn advance
	// (starts at 0 and increments after each player's move)
	UPROPERTY(BlueprintReadOnly)
	int HalfMoveCount = 0;
	
	// Total number of moves played in the game
	// (starts at 1 and increments after black's move)
	UPROPERTY(BlueprintReadOnly)
	int FullMoveCount = 0;
};
