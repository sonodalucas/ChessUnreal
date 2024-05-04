// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BCPlayerController.generated.h"

class ABCBaseGameMode;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class BUGCHESS_API ABCPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReturnAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float MoveInputCooldown = .1f;

protected:
	UPROPERTY()
	ABCBaseGameMode* GameMode;

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void Confirm();

	UFUNCTION()
	void Return();

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void MoveCompleted(const FInputActionValue& Value);

	UFUNCTION()
	void UnlockMoveInput();
	
private:
	FVector2d currentPosition;

	FTimerHandle moveCooldownHandle;

	bool moveCooldown = false;

	FVector2d inputVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float lerpAnalogFactor = 8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float analogDeadzone = .3f;
};
