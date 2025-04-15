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

// ---------------------

float lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
}

FFractionalHex hex_lerp(FHex a, FHex b, double t) {
	return FFractionalHex(
		lerp(a.Q, b.Q, t),
		lerp(a.R, b.R, t),
		lerp(a.S, b.S, t)
	);
}

std::vector<FHex> hex_linedraw(FHex a, FHex b) {
	int N = hex_distance(a, b);
	std::vector<FHex> results = {};
	double step = 1.0 / std::max(N, 1);
	for (int i = 0; i <= N; i++) {
		results.push_back(hex_round(hex_lerp(a, b, step * i)));
	}
	return results;
}

int hex_distance(FHex a, FHex b) {
	return hex_length(hex_subtract(a, b));
}

FHex hex_round(FFractionalHex h) {
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

FHex hex_subtract(FHex a, FHex b) {
	return FHex(a.Q - b.Q, a.R - b.R, a.S - b.S);
}

int hex_length(FHex hex) {
	return int((abs(hex.Q) + abs(hex.R) + abs(hex.S)) / 2);
}