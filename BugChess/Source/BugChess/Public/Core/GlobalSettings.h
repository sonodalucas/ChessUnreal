// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BCImportantTypes.h"
#include "UObject/Object.h"
#include "GlobalSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BUGCHESS_API UGlobalSettings : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,  Category = "GlobalSettings|Data Tables")
	UDataTable* GetPiecesDataAsset() const { return PiecesDataAsset; }

	UFUNCTION(BlueprintPure,  Category = "GlobalSettings|Pieces")
	FLinearColor GetPieceColour(EChessColour colour) const;

	UFUNCTION(BlueprintPure,  Category = "GlobalSettings|Pieces")
	FRotator GetPieceRotator(EChessColour colour) const;
	
private:
	UPROPERTY(EditAnywhere, Category = "GlobalSettings|Data Tables")
	UDataTable* PiecesDataAsset;

	UPROPERTY(EditAnywhere, Category="Pieces")
	FLinearColor WhiteColour;

	UPROPERTY(EditAnywhere, Category="Pieces")
	FLinearColor BlackColour;

	UPROPERTY(EditAnywhere, Category="Pieces")
	FRotator WhiteRotator;

	UPROPERTY(EditAnywhere, Category="Pieces")
	FRotator BlackRotator;
};
