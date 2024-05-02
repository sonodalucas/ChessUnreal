// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/BCPawn.h"


// Sets default values
ABCPawn::ABCPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABCPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

