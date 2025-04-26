// Fill out your copyright notice in the Description page of Project Settings.

#include "Carlos/GameManager.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("GameManager initialized."));
}

void UGameManager::Deinitialize()
{

	Super::Deinitialize();
}