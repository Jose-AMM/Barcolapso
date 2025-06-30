// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HexTile.h>
#include "ShipMovementComponent.generated.h"

class AHexGridManager;
class UHexGridVisualComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BARCOSIS_API UShipMovementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UShipMovementComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/* ------- METHODS -------*/
	void Initialize();

	void NextHexTileCalculator();

	void HexTilePathAnimator(float DeltaTime);

	bool CheckMovementRange(AHexTile* TargetHexTile);

	void PrintMovementRange(UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MouseTargetFunction(const FVector2D& MousePosition);

	/* ------ VARIABLES ------*/
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
	int MovementRange;

	AHexTile* CurrentHexTile;

	AHexTile* NextHexTile;

	UHexGridVisualComponent* HexGridVisual;

	FVector CurrentPos;

	FVector TargetPos;

	float Timer;

	bool bIsStillMoving = false;

	int HexPathCount;

public:
	/* ------- METHODS -------*/
	AHexTile* GetCurrentHexTile();

	void SetCurrentHexTile(AHexTile* HexTile);

	int GetHexPathCount() const { return HexPathCount; }

	void SetHexPathCount(int Var) { HexPathCount = Var; }

	/* ------ VARIABLES ------*/
	AHexGridManager* HexGridManager;

	TQueue<FHex> HexPath;

};
