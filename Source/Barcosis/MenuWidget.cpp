// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	InputMode = FInputModeGameAndUI();
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	//InputMode.SetHideCursorDuringCapture(true);
	PlayerController->SetInputMode(InputMode);
	MenuItems = GridPanel->GetAllChildren();
	Cast<UButton>(MenuItems[MenuNavigationIndex])->SetKeyboardFocus();
	//UpdateButtonState();
}

void UMenuWidget::HoverButtonWithMouse(UButton* Button)
{
	if (bCanReceiveInput == true)
	{
		//ThrowHoverSFXFunction();
		//ButtonFadeOutFunction(MenuNavigationIndex);

		for (int i = 0; i < MenuItems.Num(); i++)
		{
			if (Cast<UButton>(MenuItems[i]) == Button)
			{
				MenuNavigationIndex = i;
				//UpdateButtonState();
				break;	// para salir en cuanto lo encuentre
			}
		}
	}
}