// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MenuManager.generated.h"

UCLASS()
class BARCOSIS_API AMenuManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void BeginWidget();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> Widget;

};
