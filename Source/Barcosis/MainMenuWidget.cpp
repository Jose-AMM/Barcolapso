// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{

	Super::NativeConstruct();

	PrepareMainMenuControls();
}

void UMainMenuWidget::PrepareMainMenuControls()
{
	if (NewGameButton)
	{
		NewGameButton->OnPressed.AddDynamic(this, &UMainMenuWidget::NewGameFunction);
		NewGameButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnNewGameButtonHovered);
	}
	if (OptionsButton)
	{
		OptionsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::OptionsFunction);
		OptionsButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnOptionsButtonHovered);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnPressed.AddDynamic(this, &UMainMenuWidget::QuitGameFunction);
		QuitGameButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnQuitGameButtonHovered);
	}
	if (CreditsButton)
	{
		CreditsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::CreditsFunction);
		CreditsButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnCreditsButtonHovered);
	}
}

void UMainMenuWidget::OnNewGameButtonHovered()
{
	HoverButtonWithMouse(NewGameButton);
}

void UMainMenuWidget::OnOptionsButtonHovered()
{
	HoverButtonWithMouse(OptionsButton);
}

void UMainMenuWidget::OnQuitGameButtonHovered()
{
	HoverButtonWithMouse(QuitGameButton);
}

void UMainMenuWidget::OnCreditsButtonHovered()
{
	HoverButtonWithMouse(CreditsButton);
}

void UMainMenuWidget::NewGameFunction()
{
	FName Level = "L_Main";					// Nombre del nivel que carga con el botón. Este debe estar escrito exactamente igual.
	UGameplayStatics::OpenLevel(GetWorld(), Level);
}

void UMainMenuWidget::OptionsFunction()
{
	// cambiar escena
}

void UMainMenuWidget::QuitGameFunction()
{
	FGenericPlatformMisc::RequestExit(false);	// Con 'false' no requiere de confirmacion extra, como seria el caso con 'true'.
}

void UMainMenuWidget::CreditsFunction()
{
	// cambiar escena
}
