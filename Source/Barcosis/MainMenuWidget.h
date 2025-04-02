// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class BARCOSIS_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	/* ------ MÉTODOS ------*/
	virtual void NativeConstruct() override;

	void PrepareMainMenuControls();

	UFUNCTION()
	void NewGameFunction();

	UFUNCTION()
	void OptionsFunction();

	UFUNCTION()
	void CreditsFunction();

	UFUNCTION()
	void QuitGameFunction();

	UFUNCTION()
	void OnNewGameButtonHovered();

	UFUNCTION()
	void OnOptionsButtonHovered();

	UFUNCTION()
	void OnCreditsButtonHovered();

	UFUNCTION()
	void OnQuitGameButtonHovered();

	/* ------ VARIABLES ------*/
	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreditsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

};
