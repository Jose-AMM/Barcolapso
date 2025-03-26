// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "HexGridManager.generated.h"

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


UCLASS()
class BARCOSIS_API AHexGridManager : public AActor
{
	GENERATED_BODY()

private:
	// ---------- offset system ----------
	TArray<TArray<AHexTile*>> HexGrid2DArray;
	float TileHorizontalOffset;
	float OddRowHorizontalOffset;
	float TileVerticalOffset;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridHeight;

	// ---------- cube system ----------
	FOrientation LayoutPointy;
	FOrientation LayoutFlat;
	TArray <FHex> HexList;
	
	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	int32 GridRings;

	// ---------- both systems ----------
	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	float HexSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	EHexCoordsSystem CoordsSystem;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AHexTile> DefaultHexTile;
	
public:
	AHexGridManager();

protected:
	virtual void BeginPlay() override;
	void BuildOffsetHexGrid();
	void BuildCubeHexGrid();
	void InstantiateCubeHexGrid(TSubclassOf<AHexTile> tileToSpawn, FVector2D pos, FVector hex);
	FVector2D HexToPixel(FLayout layout, FVector hex);

};
