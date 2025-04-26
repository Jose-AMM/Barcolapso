// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "BattleSpawnLocation.generated.h"

/**
 * 
 */
UCLASS()
class BARCOSIS_API ABattleSpawnLocation : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	// Simple ID para encontrarlos después
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Name")
	FString SpawnLocationName = "EnemyA";
};
