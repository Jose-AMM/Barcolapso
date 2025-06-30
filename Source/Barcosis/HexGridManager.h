// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "HexGridManager.generated.h"

class UHexGridVisualComponent;

UENUM()
enum class EHexCoordsSystem : uint8
{
	OFFSET_SYSTEM,
	CUBE_SYSTEM,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FOrientation
{
	GENERATED_BODY()

public:
	double F0, F1, F2, F3;
	double B0, B1, B2, B3;
	double StartAngle;

	FOrientation() : F0(0), F1(0), F2(0), F3(0), B0(0), B1(0), B2(0), B3(0), StartAngle(0)
	{
	}

	FOrientation(double F0_, double F1_, double F2_, double F3_,
		double B0_, double B1_, double B2_, double B3_,
		double StartAngle_)
		: F0(F0_), F1(F1_), F2(F2_), F3(F3_),
		B0(B0_), B1(B1_), B2(B2_), B3(B3_),
		StartAngle(StartAngle_)
	{
	}
};


USTRUCT(BlueprintType)
struct FLayout
{
	GENERATED_BODY()

public:
	FOrientation Orientation;
	FVector2D Size;
	FVector2D Origin;

	FLayout()
		: Orientation(), Size(FVector2D::ZeroVector), Origin(FVector2D::ZeroVector)
	{
	}

	FLayout(FOrientation Orientation_, FVector2D Size_, FVector2D Origin_)
		: Orientation(Orientation_), Size(Size_), Origin(Origin_)
	{
	}
};

class UShipMovementComponent;

UCLASS()
class BARCOSIS_API AHexGridManager : public AActor
{
	GENERATED_BODY()

public:
	AHexGridManager();

protected:
	virtual void BeginPlay() override;

private:
	/* ------- METHODS -------*/
	void BuildOffsetHexGrid();

	void BuildCubeHexGrid();

	void InstantiateCubeHexGrid(TSubclassOf<AHexTile> TileToSpawn, FVector2D Pos, FVector Hex);

	FVector2D HexToPixel(FLayout Layout, FVector Hex);

	bool IsANeighboringHexTile(AHexTile* HexTile);

	//void OriginalHexTileMaterialReverter(float DeltaTime);

	/* ------ VARIABLES ------*/
	/* ---- offset system ----*/
	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridHeight;

	TArray<TArray<AHexTile*>> HexGrid2DArray;

	float TileHorizontalOffset;

	float OddRowHorizontalOffset;

	float TileVerticalOffset;

	/* ----- cube system -----*/
	FOrientation LayoutPointy;

	FOrientation LayoutFlat;

	TArray <FHex> HexList;

	TArray <AHexTile*> HexTileList;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridRings;

	/* ----- both system -----*/
	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	float HexSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	EHexCoordsSystem CoordsSystem;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AHexTile> DefaultHexTile;

	UPROPERTY(EditAnywhere, Category = "Timer|Time")
	float TargetedTime = 5.0f;

	const TArray<FHex> HexDirections = {
		FHex(1, 0, -1), FHex(1, -1, 0), FHex(0, -1, 1),
		FHex(-1, 0, 1), FHex(-1, 1, 0), FHex(0, 1, -1)
	};

	APawn* Player;

	AHexTile* TargetedHexTile;

	UShipMovementComponent* ShipMovement;

	UHexGridVisualComponent* HexGridVisual;

	bool bIsHexTileTargeted = false;

	float Timer = 0.0f;

public:
	/* ------- METHODS -------*/
	AHexTile* GetHexTile(FHex Hex);

	const UHexGridVisualComponent* GetHexGridVisual() const { return HexGridVisual; }

	/* ----- hex library -----*/
	float Lerp(double A, double B, double T);

	FFractionalHex HexLerp(FHex A, FHex B, double T);

	void HexLinedraw(FHex A, FHex B);

	int HexDistance(FHex A, FHex B);

	FHex HexRound(FFractionalHex H);

	FHex HexSubtract(FHex A, FHex B);

	int HexLength(FHex Hex);

	/* ------ VARIABLES ------*/

};
