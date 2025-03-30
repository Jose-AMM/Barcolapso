// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class BARCOSIS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Called every frame
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	/* ------ MÉTODOS ------*/
	virtual void NativeConstruct() override;

	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//void UpdateButtonState();

	//UFUNCTION(BlueprintCallable)
	//void UpdateButtonOpacity(UButton* Button, float Alpha);

	void HoverButtonWithMouse(UButton* Button);

	/* ------ VARIABLES ------*/
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GridPanel;

	static bool b_ComesFromMainMenu;

	static bool bCanPressAccept;

	UPROPERTY()
	int MenuNavigationIndex = 0;

	UPROPERTY()
	float NavigateMenuTimer;

	UPROPERTY()
	float NavigationTime = 0.1f;

	//UPROPERTY()
	//bool bIsNavigationButtonPressed = false;

	UPROPERTY()
	bool bCanMoveNextButton = true;

	UPROPERTY(BlueprintReadWrite)
	bool bCanReceiveInput = true;

	UPROPERTY()
	APlayerController* PlayerController;

	FInputModeGameAndUI InputMode;

	UPROPERTY()
	TArray<UWidget*> MenuItems;
};
