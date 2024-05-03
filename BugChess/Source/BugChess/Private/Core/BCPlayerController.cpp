// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Core/BCBaseGameMode.h"


void ABCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABCBaseGameMode>(GetWorld()->GetAuthGameMode());
	CurrentPosition = FVector2d::Zero();
}

void ABCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
	    Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &ABCPlayerController::Confirm);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABCPlayerController::Move);
		
		EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Triggered, this, &ABCPlayerController::Return);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ABCPlayerController::Confirm()
{
	const auto Grid = GameMode->GetBoard()->GetGrid();

	const auto Piece = Cast<UChessCellObject>(Grid->GetGridObject(CurrentPosition.X, CurrentPosition.Y))->GetPiece();

	if (!Piece)
	{
		UE_LOG(LogTemp, Log, TEXT("No piece at [%.0f, %.0f]."), CurrentPosition.X, CurrentPosition.Y)
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[%s %s] at [%.0f, %.0f]"), *UEnum::GetDisplayValueAsText(Piece->GetColour()).ToString(),
		*UEnum::GetDisplayValueAsText(Piece->GetType()).ToString(), CurrentPosition.X, CurrentPosition.Y)

}

void ABCPlayerController::Return()
{
	UE_LOG(LogTemp, Warning, TEXT("Return"))
}

void ABCPlayerController::Move(const FInputActionValue& Value)
{
	if (MoveCooldown) return;
	MoveCooldown = true;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	int PositionX = FMath::Clamp(CurrentPosition.X + MovementVector.X, 0, 7);
	int PositionY = FMath::Clamp(CurrentPosition.Y + MovementVector.Y, 0, 7);
	
	CurrentPosition = FVector2d(PositionX, PositionY);

	const auto Grid = GameMode->GetBoard()->GetGrid();
	const auto WorldPosition = Grid->GetCellCenterWorldPosition(CurrentPosition.X, CurrentPosition.Y);

	DrawDebugBox(GetWorld(), WorldPosition, FVector(Grid->GetCellSize() / 2, Grid->GetCellSize() / 2, 10),
		FColor::Cyan, false, 1.f, 0, 10);

	GetWorld()->GetTimerManager().SetTimer(MoveCooldownHandle, this, &ABCPlayerController::UnlockMoveInput, MoveInputCooldown);
}

void ABCPlayerController::UnlockMoveInput()
{
	MoveCooldown = false;
}
