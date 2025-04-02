// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Const.generated.h"

/**
 * 
 */
UCLASS()
class BARCOSIS_API UConst : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* ------ MÉTODOS ------*/
	UFUNCTION(BlueprintPure, Category = "Game Constants")
	static FName GetMainLevel() { return MainLevel; }

	UFUNCTION(BlueprintPure, Category = "Game Constants")
	static FName GetOptionsLevel() { return OptionsLevel; }

	UFUNCTION(BlueprintPure, Category = "Game Constants")
	static FName GetCreditsLevel() { return CreditsLevel; }

	/* ------ VARIABLES ------*/
	static const FName MainLevel;
	static const FName OptionsLevel;
	static const FName CreditsLevel;

};
