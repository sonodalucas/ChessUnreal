// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/BCPiece.h"

#include "Library/BCFunctionLibrary.h"


// Sets default values
ABCPiece::ABCPiece()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ABCPiece::SetupPiece(EChessColour Colour, UStaticMesh* Mesh)
{
	StaticMesh->SetStaticMesh(Mesh);
	SetPieceColour(Colour);
}

void ABCPiece::SetPieceColour(EChessColour colour) const
{
	FLinearColor PieceColour = UBCFunctionLibrary::GetGlobalSettings()->GetPieceColour(colour);
	DynamicMaterial->SetVectorParameterValue(FName("Colour"), PieceColour);
}

// Called when the game starts or when spawned
void ABCPiece::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMesh->SetMaterial(0, DynamicMaterial);
}
