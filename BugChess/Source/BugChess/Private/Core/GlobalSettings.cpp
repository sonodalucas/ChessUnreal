// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GlobalSettings.h"

FLinearColor UGlobalSettings::GetPieceColour(EChessColour colour) const
{
	return colour == EChessColour::ECC_White ? WhiteColour : BlackColour;
}
