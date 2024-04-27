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

UCLASS()
class BUGCHESS_API UBCImportantTypes : public UObject
{
	GENERATED_BODY()
};
