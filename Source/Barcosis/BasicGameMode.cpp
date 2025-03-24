// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"

ABasicGameMode::ABasicGameMode()
{
}

void ABasicGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Log a message when the level starts
    UE_LOG(LogTemp, Log, TEXT("START"));
}