// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Components/SceneComponent.h"
#include "Pieces/BCPiece.h"
#include "BoardComponent.generated.h"

class ABCBaseGameMode;
DECLARE_LOG_CATEGORY_EXTERN(LogBoard, Log, All);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUGCHESS_API UBoardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	
	// Sets default values for this component's properties
	UBoardComponent();

	void StartBoard(FBoardInfo BoardInfo);

	UGrid* GetGrid() const { return Grid; }

	UFUNCTION(BlueprintNativeEvent)
	bool MovePiece(UChessCellObject* OriginCell, UChessCellObject* DestinationCell);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SpawnPiece(EChessPiece ChessPiece, EChessColour Colour, int Position, int PositionY) const;
	
	UPROPERTY(VisibleAnywhere)
	UGrid* Grid;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABCPiece> PieceClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCellObject> CellClass;

	uint64& GetPieceTypeBitBoard(EChessPiece PieceType, EChessColour PlayerColour);

	uint64 GetPlayerBitBoard(EChessColour PlayerColour) const;

	uint64 GetAllPiecesBitBoard() const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

#pragma region Debug
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	bool showDebug;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, EditConditionHides, EditCondition=showDebug))
	TEnumAsByte<EChessColour> ColourToDebug;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, EditConditionHides, EditCondition=showDebug))
	TEnumAsByte<EChessPiece> PieceToDebug;

	void PrintBitboard();
#pragma endregion 

	UPROPERTY()
	ABCBaseGameMode* gameMode;

#pragma region BitBoard
	uint64 whiteRooksPositions = 0;

	uint64 whiteBishopsPositions = 0;

	uint64 whiteKnightsPositions = 0;

	uint64 whiteQueensPositions = 0;

	uint64 whiteKingsPositions = 0;

	uint64 whitePawnsPositions = 0;
	
	uint64 blackRooksPositions = 0;

	uint64 blackBishopsPositions = 0;

	uint64 blackKnightsPositions = 0;

	uint64 blackQueensPositions = 0;

	uint64 blackKingsPositions = 0;

	uint64 blackPawnsPositions = 0;

#pragma endregion

#pragma region BoardInfo
	// Castling
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool whiteCastleKingSide = true;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool whiteCastleQueenSide = true;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool blackCastleKingSide = true;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool blackCastleQueenSide = true;
	
	// En passant file (0 is a-file, 7 is h-file, -1 means none)
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int epFile = -1;
#pragma endregion 
};
