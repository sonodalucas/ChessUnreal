// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Components/SceneComponent.h"
#include "Pieces/BCPiece.h"
#include "BoardComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBoard, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUGCHESS_API UBoardComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoardComponent();

	void StartBoard();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SpawnPiece(EChessPiece ChessPiece, EChessColour Colour, int Position, int PositionY) const;
	
	UPROPERTY(VisibleAnywhere)
	UGrid* Grid;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABCPiece> PieceClass;
};
