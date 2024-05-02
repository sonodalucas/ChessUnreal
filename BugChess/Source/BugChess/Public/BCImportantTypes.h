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
	ECC_None = 0		UMETA(DisplayName="None"),
	ECC_White = 1		UMETA(DisplayName="White"),
	ECC_Black = 2		UMETA(DisplayName="Black")
};

UENUM(BlueprintType)
enum EChessPiece
{
	ECP_None = 0		UMETA(DisplayName="None"),
	ECP_Pawn = 1		UMETA(DisplayName="Pawn"),
	ECP_Knight = 2		UMETA(DisplayName="Knight"),
	ECP_Bishop = 3		UMETA(DisplayName="Knight"),
	ECP_Rook = 3		UMETA(DisplayName="Rook"),
	ECP_Queen = 3		UMETA(DisplayName="Queen"),
	ECP_King = 3		UMETA(DisplayName="King")
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
