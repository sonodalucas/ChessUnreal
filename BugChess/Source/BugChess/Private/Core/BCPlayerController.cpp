// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Core/BCBaseGameMode.h"


void ABCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABCBaseGameMode>(GetWorld()->GetAuthGameMode());
	currentPosition = FVector2d::Zero();
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABCPlayerController::MoveCompleted);
		
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

	const auto Piece = Cast<UChessCellObject>(Grid->GetGridObject(currentPosition.X, currentPosition.Y))->GetPiece();

	if (!Piece)
	{
		UE_LOG(LogTemp, Log, TEXT("No piece at [%.0f, %.0f]."), currentPosition.X, currentPosition.Y)
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[%s %s] at [%.0f, %.0f]"), *UEnum::GetDisplayValueAsText(Piece->GetColour()).ToString(),
		*UEnum::GetDisplayValueAsText(Piece->GetType()).ToString(), currentPosition.X, currentPosition.Y)

}

void ABCPlayerController::Return()
{
	UE_LOG(LogTemp, Warning, TEXT("Return"))
}

void ABCPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const float XLerp = FMath::Lerp(inputVector.X, MovementVector.X, lerpAnalogFactor * GetWorld()->GetDeltaSeconds());
	const float YLerp = FMath::Lerp(inputVector.Y, MovementVector.Y, lerpAnalogFactor * GetWorld()->GetDeltaSeconds());
	
	inputVector.X = MovementVector.X == 0 ? 0 : XLerp;
	inputVector.Y = MovementVector.Y == 0 ? 0 : YLerp;
	
	if (moveCooldown || analogDeadzone > inputVector.Length()) return;
	moveCooldown = true;
	
	UE_LOG(LogTemp, Warning, TEXT("Input vector [%s]"), *inputVector.ToString())
	int PositionX = FMath::Clamp(currentPosition.X + FMath::RoundFromZero(inputVector.X), 0, 7);
	int PositionY = FMath::Clamp(currentPosition.Y + FMath::RoundFromZero(inputVector.Y), 0, 7);
	
	currentPosition = FVector2d(PositionX, PositionY);

	UE_LOG(LogTemp, Warning, TEXT("Current position [%s]"), *currentPosition.ToString())

	
	const auto Grid = GameMode->GetBoard()->GetGrid();
	const auto WorldPosition = Grid->GetCellCenterWorldPosition(currentPosition.X, currentPosition.Y);

	DrawDebugBox(GetWorld(), WorldPosition, FVector(Grid->GetCellSize() / 2, Grid->GetCellSize() / 2, 10),
		FColor::Cyan, false, 1.f, 0, 10);
	
	GetWorld()->GetTimerManager().SetTimer(moveCooldownHandle, this, &ABCPlayerController::UnlockMoveInput, MoveInputCooldown);
}

void ABCPlayerController::MoveCompleted(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if(MovementVector.Length() == 0)
	{
		inputVector = FVector2d::Zero();
		UE_LOG(LogTemp, Warning, TEXT("Movement vector [%s]"), *MovementVector.ToString())
	}
}

void ABCPlayerController::UnlockMoveInput()
{
	moveCooldown = false;
}
