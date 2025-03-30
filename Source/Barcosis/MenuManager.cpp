// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManager.h"

// Sets default values
AMenuManager::AMenuManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMenuManager::BeginPlay()
{
	Super::BeginPlay();
	
	BeginWidget();
}

void AMenuManager::BeginWidget()
{
	if (Widget)
	{
		auto Menu = CreateWidget<UUserWidget>(GetWorld(), Widget);
        if (Menu)
        {
			Menu->AddToViewport();
        }
	}
}
