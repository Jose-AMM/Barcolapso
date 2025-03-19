// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BARCOSIS_API ABasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    // Constructor
    ABasicGameMode();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
