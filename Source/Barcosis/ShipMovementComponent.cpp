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

void UShipMovementComponent::MoveToTargetHexTile(FHex TargetHexTile)
{
	Timer = 0.0f;
	CurrentPos = CurrentHexTile->GetOwner()->GetActorLocation();
	//TargetPos = TargetHexTile->GetOwner()->GetActorLocation();
	bMoveFlag = true;
}

void UShipMovementComponent::Recorrido(AHexTile* HexTarget)
{
	std::vector<FHex> Vector = Hex_linedraw(CurrentHexTile->Hex, HexTarget->Hex);

	for (int i = 0; i < Vector.size(); ++i)
	{
		MoveToTargetHexTile(Vector[i]);
	}
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

// ---------------------

float UShipMovementComponent::Lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
}

FFractionalHex UShipMovementComponent::Hex_lerp(FHex a, FHex b, double t)
{
	return FFractionalHex(
		Lerp(a.Q, b.Q, t),
		Lerp(a.R, b.R, t),
		Lerp(a.S, b.S, t)
	);
}

std::vector<FHex> UShipMovementComponent::Hex_linedraw(FHex a, FHex b)
{
	int N = Hex_distance(a, b);
	std::vector<FHex> results = {};
	double step = 1.0 / std::max(N, 1);
	for (int i = 0; i <= N; i++) {
		results.push_back(Hex_round(Hex_lerp(a, b, step * i)));
	}
	return results;
}

int UShipMovementComponent::Hex_distance(FHex a, FHex b)
{
	return Hex_length(Hex_subtract(a, b));
}

FHex UShipMovementComponent::Hex_round(FFractionalHex h)
{
	int q = int(round(h.Q));
	int r = int(round(h.R));
	int s = int(round(h.S));
	double q_diff = abs(q - h.Q);
	double r_diff = abs(r - h.R);
	double s_diff = abs(s - h.S);
	if (q_diff > r_diff and q_diff > s_diff) {
		q = -r - s;
	}
	else if (r_diff > s_diff) {
		r = -q - s;
	}
	else {
		s = -q - r;
	}
	return FHex(q, r, s);
}

FHex UShipMovementComponent::Hex_subtract(FHex a, FHex b)
{
	return FHex(a.Q - b.Q, a.R - b.R, a.S - b.S);
}

int UShipMovementComponent::Hex_length(FHex hex)
{
	return int((abs(hex.Q) + abs(hex.R) + abs(hex.S)) / 2);
}