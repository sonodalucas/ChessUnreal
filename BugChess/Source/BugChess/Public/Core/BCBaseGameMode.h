// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Board/BoardComponent.h"
#include "GameFramework/GameMode.h"
#include "BCBaseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnEvent, EChessColour, Colour);

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

	UFUNCTION()
	void Confirm(UChessCellObject* CellObject);

	UFUNCTION()
	void ChoosePiece(UChessCellObject* CellObject);

	UFUNCTION()
	void MovePiece(UChessCellObject* DestinationCell);

	UFUNCTION()
	void UnselectPiece();

protected:
	UPROPERTY(EditAnywhere)
	UBoardComponent* Board;

private:
	UPROPERTY()
	TEnumAsByte<EChessColour> CurrentTurn;

	UPROPERTY()
	ABCPiece* PieceSelected;

	void StartGame();

	UFUNCTION()
	void StartTurn();

	UFUNCTION()
	void EndTurn();
};
