// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BCImportantTypes.h"
#include "GameFramework/Actor.h"
#include "BCPiece.generated.h"

UCLASS()
class BUGCHESS_API ABCPiece : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABCPiece();

	void SetupPiece(EChessPiece PieceType, EChessColour PieceColour, UStaticMesh* Mesh);

	EChessPiece GetType() const { return Type; }

	EChessColour GetColour() const { return Colour; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetPieceColour(EChessColour colour) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialInterface;

	EChessPiece Type;

	EChessColour Colour;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
