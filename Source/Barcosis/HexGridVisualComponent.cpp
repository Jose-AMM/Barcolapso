// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridVisualComponent.h"

// Sets default values for this component's properties
UHexGridVisualComponent::UHexGridVisualComponent()
{
}


// Called when the game starts
void UHexGridVisualComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UHexGridVisualComponent::HexTilePainter(AHexTile* HexTile, const UMaterialInterface* Material)
{
	UStaticMeshComponent* MeshComponent = HexTile->FindComponentByClass<UStaticMeshComponent>();

	if (MeshComponent && Material)
	{
		if (Material)
		{
			MeshComponent->SetMaterial(0, const_cast<UMaterialInterface*>(Material));
		}
	}
}
