// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BCImportantTypes.h"
#include "GameFramework/PlayerState.h"
#include "BCPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BUGCHESS_API ABCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetPlayerColour(EChessColour PlayerColour);

	EChessColour GetPlayerColour() const { return Colour; }
	
private:
	UPROPERTY()
	TEnumAsByte<EChessColour> Colour;
};
