// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Board/BoardComponent.h"
#include "GameFramework/GameMode.h"
#include "BCBaseGameMode.generated.h"

class ABCBaseGameState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnEvent, bool, WhiteToMove);

#define STARTING_LAYOUT TEXT("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")

/**
 * 
 */
UCLASS()
class BUGCHESS_API ABCBaseGameMode : public AGameMode
{
	GENERATED_BODY()

	ABCBaseGameMode();

	virtual void BeginPlay() override;

public:
	UPROPERTY()
	FOnTurnEvent OnTurnStarted;

	FOnTurnEvent OnTurnEnded;
	
	UBoardComponent* GetBoard() const { return Board; }

	TEnumAsByte<EChessColour> GetActiveColour() const { return whiteToMove ? EChessColour::ECC_White : EChessColour::ECC_Black; }

	UFUNCTION()
	void Confirm(UChessCellObject* CellObject);

	UFUNCTION()
	void ChoosePiece(UChessCellObject* CellObject);

	UFUNCTION()
	void MovePiece(UChessCellObject* DestinationCell);

	UFUNCTION()
	void UnselectCell();

protected:
	UPROPERTY(EditAnywhere)
	UBoardComponent* Board;

	UPROPERTY(EditAnywhere)
	FString FenString;
	
private:
	UPROPERTY()
	bool whiteToMove;

	UPROPERTY()
	UChessCellObject* CellSelected;

	void StartGame();

	UFUNCTION()
	void StartTurn();

	UFUNCTION()
	void EndTurn() const;
};
