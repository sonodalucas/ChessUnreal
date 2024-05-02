// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/BCFunctionLibrary.h"

UGlobalSettings* UBCFunctionLibrary::GetGlobalSettings()
{
	if (GEngine)
	{
		if (UGlobalSettings* DataInstance = Cast<UGlobalSettings>(GEngine->GameSingleton))
		{
			return DataInstance;
		}
	}

	return nullptr;

}
