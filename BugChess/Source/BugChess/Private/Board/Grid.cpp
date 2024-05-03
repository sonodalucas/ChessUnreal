// Fill out your copyright notice in the Description page of Project Settings.


#include "BugChess/Public/Board/Grid.h"

DEFINE_LOG_CATEGORY(LogGrid);

void UGrid::InitGrid(int height, int width, float cellSize, FVector originPosition, TSubclassOf<UCellObject> CellClass)
{
	Height = height;
	Width = width;
	OriginPosition = originPosition;
	CellSize = cellSize;

	
	// Initialize grid
	for (int i = 0; i < Height * Width; ++i)
	{
		UCellObject* CellObject = NewObject<UCellObject>(GetTransientPackage(), CellClass);
		const int x = i % Width;
		const int y = i / Width;
		CellObject->Initialize(this, x, y);
		GridArray.Add(CellObject);
		UE_LOG(LogGrid, Log, TEXT("Initialized [%d, %d] grid cell."), x, y);
	}

	if (!ShowDebug) return;

	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			DrawDebugLine(GEngine->GetCurrentPlayWorld(), GetWorldPosition(x, y), GetWorldPosition(x, y + 1), FColor::Emerald, true, -1, 0, 10);
			DrawDebugLine(GEngine->GetCurrentPlayWorld(), GetWorldPosition(x, y), GetWorldPosition(x + 1, y), FColor::Emerald, true, -1, 0, 10);

		}
	}

	DrawDebugLine(GEngine->GetCurrentPlayWorld(), GetWorldPosition(0, Height), GetWorldPosition(Width,  Height), FColor::Emerald, true, -1, 0, 10);
	DrawDebugLine(GEngine->GetCurrentPlayWorld(), GetWorldPosition(Width, 0), GetWorldPosition(Width,  Height), FColor::Emerald, true, -1, 0, 10);

}

FVector UGrid::GetWorldPosition(int X, int Y)
{
	return (FVector(Y, X, 0) * CellSize) + OriginPosition;
}

FVector UGrid::GetCellCenterWorldPosition(int X, int Y)
{
	return GetWorldPosition(X, Y) + FVector(GetCellSize() / 2, GetCellSize() / 2, 0);
}

void UGrid::GetGridPosition(FVector WorldPosition, int& X, int& Y)
{
	X = FMath::FloorToInt((WorldPosition - OriginPosition).Y / CellSize);
	Y = FMath::FloorToInt((WorldPosition - OriginPosition).X / CellSize);
}

UCellObject* UGrid::GetGridObject(int X, int Y)
{
	if (IsInsideGrid(X, Y)) return GridArray[Y * Width + X];

	UE_LOG(LogGrid, Warning, TEXT("Get object failed. There is no position [%d, %d] inside the grid."), X, Y);
	return nullptr;
}

UCellObject* UGrid::GetGridObject(FVector WorldPosition)
{
	int x;
	int y;
	GetGridPosition(WorldPosition, x, y);

	return GetGridObject(x, y);
}

void UGrid::SetGridObject(int X, int Y, UCellObject* GridObject)
{
	if (IsInsideGrid(X, Y))
	{
		GridArray[Y * Width + X] = GridObject;

		TriggerGridObjectChanged(X, Y);
	}

	UE_LOG(LogGrid, Warning, TEXT("Set object failed. There is no position [%d, %d] inside the grid."), X, Y);

}

void UGrid::SetGridObject(FVector WorldPosition, UCellObject* GridObject)
{
	int x;
	int y;
	GetGridPosition(WorldPosition, x, y);

	SetGridObject(x, y, GridObject);
}

void UGrid::TriggerGridObjectChanged(int X, int Y) const
{
	if (OnGridCellObjectChanged.IsBound())
		OnGridCellObjectChanged.Broadcast(X, Y);
}

bool UGrid::IsInsideGrid(int X, int Y) const
{
	return X >= 0 && X < Width && Y >= 0 && Y < Height;
}

void UCellObject::Initialize(UGrid* grid, int x, int y)
{
	X = x;
	Y = y;
	Grid = grid;
	
}

void UChessCellObject::SetPiece(ABCPiece* NewPiece)
{
	Piece = NewPiece;
	Grid->TriggerGridObjectChanged(X, Y);
}
