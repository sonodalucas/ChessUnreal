// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Components/SceneComponent.h"
#include "Pieces/BCPiece.h"
#include "BoardComponent.generated.h"

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

	uint64& GetPositionsOfPieceType(EChessPiece PieceType);

	uint64& GetPositionsOfPlayer(EChessColour PlayerColour);

	uint64 GetAllPiecesPosition() const;

private:
	uint64 whitePiecesPositions = 0;

	uint64 blackPiecesPositions = 0;

	uint64 rooksPositions = 0;

	uint64 bishopsPositions = 0;

	uint64 knightsPositions = 0;

	uint64 queensPositions = 0;

	uint64 kingsPositions = 0;

	uint64 pawnsPositions = 0;
};
