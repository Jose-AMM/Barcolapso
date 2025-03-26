// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.generated.h"

class UStaticMeshComponent;

UENUM()
enum class EHexTileType : uint8
{
	INVALID, 
	GRASS,
	WATER,
	MAX UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FHex
{
	GENERATED_BODY()

public:
	int Q, R, S;

	FHex() : Q(0), R(0), S(0)
	{
	}

	FHex(int Q_, int R_, int S_)
		: Q(Q_), R(R_), S(S_)
	{
		ensureAlwaysMsgf(Q + R + S == 0, TEXT("Hex coordinates must satisfy Q + R + S = 0"));
	}
};


UCLASS()
class BARCOSIS_API AHexTile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	FIntPoint TileIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	FHex Hex;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	EHexTileType TileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	UStaticMeshComponent* TileMesh;
	
public:
	AHexTile();

};
