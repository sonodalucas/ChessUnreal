// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Board/BoardComponent.h"
#include "GameFramework/GameMode.h"
#include "BCBaseGameMode.generated.h"

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
	UBoardComponent* GetBoard() const { return Board; }

protected:
	UPROPERTY(EditAnywhere)
	UBoardComponent* Board;
};
