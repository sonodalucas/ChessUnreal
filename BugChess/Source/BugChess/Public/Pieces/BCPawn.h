// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BCPiece.h"
#include "BCPawn.generated.h"

UCLASS()
class BUGCHESS_API ABCPawn : public ABCPiece
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABCPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
