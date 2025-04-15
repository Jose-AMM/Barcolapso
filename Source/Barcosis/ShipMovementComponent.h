// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HexTile.h>
#include "ShipMovementComponent.generated.h"

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
	/* ------ MÉTODOS ------*/
	void MoveToTargetHexTile(FHex TargetHexTile);

	void MoveToTargetHexTileAnimator(float DeltaTime);

	int Hex_distance(FHex a, FHex b);

	std::vector<FHex> Hex_linedraw(FHex a, FHex b);

	float Lerp(double a, double b, double t);

	FFractionalHex Hex_lerp(FHex a, FHex b, double t);

	FHex Hex_round(FFractionalHex h);

	FHex Hex_subtract(FHex a, FHex b);

	int Hex_length(FHex hex);

	void Recorrido(AHexTile* HexTarget);

	/* ------ VARIABLES ------*/
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementTime;

	AHexTile* CurrentHexTile;

	FVector CurrentPos;

	FVector TargetPos;

	float Timer;

	bool bMoveFlag = false;

public:
	/* ------ MÉTODOS ------*/
	AHexTile* GetCurrentHexTile();

	void SetCurrentHexTile(AHexTile* HexTile);

	/* ------ VARIABLES ------*/

};
