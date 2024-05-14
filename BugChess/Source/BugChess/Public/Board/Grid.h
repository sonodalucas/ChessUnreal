// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BugChess/Public/BCImportantTypes.h"
#include "UObject/Object.h"
#include "Grid.generated.h"

class UCellObject;
class ABCPiece;
DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGridCell, int, PositionX, int, PositionY);

/**
 * 
 */

UCLASS()
class BUGCHESS_API UGrid : public UObject
{
	GENERATED_BODY()

public:
	FOnGridCell OnGridCellObjectChanged;
	
	void InitGrid(int height, int width, float cellSize, FVector originPosition, TSubclassOf<UCellObject> CellClass);
	
	UFUNCTION(BlueprintCallable)
	FVector GetWorldPosition(int X, int Y);

	UFUNCTION(BlueprintCallable)
	FVector GetCellCenterWorldPosition(int X, int Y);

	UFUNCTION(BlueprintCallable)
	void GetGridPosition(FVector WorldPosition, int& X, int& Y);

	UFUNCTION(BlueprintCallable)
	UCellObject* GetGridObject(int X, int Y);
	UCellObject* GetGridObject(FVector WorldPosition);

	UFUNCTION(BlueprintCallable)
	void SetGridObject(int X, int Y, UCellObject* GridObject);
	void SetGridObject(FVector WorldPosition, UCellObject* GridObject);

	void TriggerGridObjectChanged(int X, int Y) const;

	bool IsInsideGrid(int X, int Y) const;

	UFUNCTION(BlueprintCallable)
	int GetWidth() const { return Width; }

	UFUNCTION(BlueprintCallable)
	int GetHeight() const { return Height; }

	UFUNCTION(BlueprintCallable)
	float GetCellSize() const { return CellSize; }
	
protected:
	UPROPERTY(EditAnywhere)
	bool ShowDebug = true;

private:
	UPROPERTY(VisibleAnywhere, Category="Debug")
	TArray<UCellObject*> GridArray;

	UPROPERTY(VisibleAnywhere, Category="Debug")
	float CellSize;

	// (0, 0) of the grid
	UPROPERTY(VisibleAnywhere, Category="Debug")
	FVector OriginPosition;

	UPROPERTY(VisibleAnywhere, Category="Debug")
	int Height;

	UPROPERTY(VisibleAnywhere, Category="Debug")
	int Width;

	
};

UCLASS()
class UCellObject : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UGrid* grid, int x, int y);

	UFUNCTION(BlueprintPure)
	FVector2D GetPositionOnGrid() const { return FVector2D(X, Y); }

protected:
	UPROPERTY()
	UGrid* Grid;

	UPROPERTY(VisibleAnywhere, Category="Debug")
	int X;

	UPROPERTY(VisibleAnywhere, Category="Debug")
	int Y;
};

UCLASS()
class UChessCellObject : public UCellObject
{
	GENERATED_BODY()

public:
	void SetPiece(ABCPiece* NewPiece);

	ABCPiece* GetPiece() const { return Piece;}

protected:
	UPROPERTY(VisibleAnywhere)
	ABCPiece* Piece;
};
