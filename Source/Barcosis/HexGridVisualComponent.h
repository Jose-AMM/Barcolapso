// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInterface.h"
#include "HexGridVisualComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BARCOSIS_API UHexGridVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexGridVisualComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* ------- METHODS -------*/

	/* ------ VARIABLES ------*/
	UPROPERTY(EditAnywhere, Category = "HexGrid|Material")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Material")
	UMaterialInterface* PathMaterial;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Material")
	UMaterialInterface* TargetMaterial;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Material")
	UMaterialInterface* RangeMaterial;

public:
	/* ------- METHODS -------*/
	void HexTilePainter(AHexTile* HexTile, const UMaterialInterface* Material);

	/* ------ VARIABLES ------*/
	const UMaterialInterface* GetDefaultMaterial() const { return DefaultMaterial; }

	const UMaterialInterface* GetPathMaterial() const { return PathMaterial; }

	const UMaterialInterface* GetTargetMaterial() const { return TargetMaterial; }

	const UMaterialInterface* GetRangeMaterial() const { return RangeMaterial; }

};
