// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"

// Sets default values
UShipMovementComponent::UShipMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveToTargetHexTileAnimator(DeltaTime);
}

void UShipMovementComponent::MoveToTargetHexTile()
{
	Timer = 0.0f;
	CurrentPos = CurrentHexTile->GetOwner()->GetActorLocation();
	TargetPos = TargetHexTile->GetOwner()->GetActorLocation();
	bMoveFlag = true;
}

void UShipMovementComponent::MoveToTargetHexTileAnimator(float DeltaTime)
{
	if (bMoveFlag)
	{
		Timer += DeltaTime;
		if (Timer > MovementTime)
		{
			Timer = MovementTime;
			bMoveFlag = false;
		}
		FVector Position = FMath::Lerp(CurrentPos, TargetPos, Timer / MovementTime);
		GetOwner()->SetActorLocation(Position);
	}
}

AHexTile* UShipMovementComponent::GetCurrentHexTile()
{
	return CurrentHexTile;
}

void UShipMovementComponent::SetCurrentHexTile(AHexTile* HexTile)
{
	CurrentHexTile = HexTile;
}

