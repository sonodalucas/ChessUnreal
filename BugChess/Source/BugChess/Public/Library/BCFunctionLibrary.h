// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GlobalSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BCFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BUGCHESS_API UBCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static UGlobalSettings* GetGlobalSettings();
};
