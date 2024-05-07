// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Core/BCBaseGameMode.h"
#include "Core/CustomLogging.h"
#include "Core/BCPlayerState.h"


void ABCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABCBaseGameMode>(GetWorld()->GetAuthGameMode());
	currentPosition = FVector2d::Zero();

	GameMode->OnTurnStarted.AddDynamic(this, &ABCPlayerController::StartPlayerTurn);
	GameMode->OnTurnEnded.AddDynamic(this, &ABCPlayerController::EndPlayerTurn);
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
	const auto Cell = Cast<UChessCellObject>(Grid->GetGridObject(currentPosition.X, currentPosition.Y));

	GameMode->Confirm(Cell);

	const auto Piece = Cell->GetPiece();

	if (!Piece)
	{
		UE_LOGFMT(LogChessPlayerController, Log, "No piece at [{0}, {1}].", currentPosition.X, currentPosition.Y);
		return;
	}

	UE_LOGFMT(LogChessPlayerController, Log, "[{0} {1}] at [{2}, {3}]", *UEnum::GetDisplayValueAsText(Piece->GetColour()).ToString(),
		*UEnum::GetDisplayValueAsText(Piece->GetType()).ToString(), currentPosition.X, currentPosition.Y);

}

void ABCPlayerController::Return()
{
	GameMode->UnselectPiece();
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
	
	// UE_LOG(LogTemp, Warning, TEXT("Input vector [%s]"), *inputVector.ToString())
	int PositionX = FMath::Clamp(currentPosition.X + FMath::RoundFromZero(inputVector.X), 0, 7);
	int PositionY = FMath::Clamp(currentPosition.Y + FMath::RoundFromZero(inputVector.Y), 0, 7);
	
	currentPosition = FVector2d(PositionX, PositionY);

	// UE_LOG(LogTemp, Warning, TEXT("Current position [%s]"), *currentPosition.ToString())

	
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

void ABCPlayerController::StartPlayerTurn(EChessColour Colour)
{
	// TODO: Reactivate this when the AIC is implemented
	// if (Cast<ABCPlayerState>(PlayerState)->GetPlayerColour() == Colour)
	// {
	// 	EnableInput(this);
	// }
}

void ABCPlayerController::EndPlayerTurn(EChessColour Colour)
{
	// TODO: Reactivate this when the AIC is implemented
	// if (Cast<ABCPlayerState>(PlayerState)->GetPlayerColour() == Colour)
	// {
	// 	DisableInput(this);
	// }
}
