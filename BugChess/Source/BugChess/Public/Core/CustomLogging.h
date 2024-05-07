// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Logging/StructuredLog.h"
#include "CustomLogging.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChessGameMode, Log, All)

DECLARE_LOG_CATEGORY_EXTERN(LogChessPlayerController, Log, All)

/**
 * 
 */
UCLASS()
class BUGCHESS_API UCustomLogging : public UObject
{
	GENERATED_BODY()
};
