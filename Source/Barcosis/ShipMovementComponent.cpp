// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"
#include "HexGridManager.h"

// Sets default values
UShipMovementComponent::UShipMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	//PrintCurrentMovementRange();
}

// Called every frame
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HexTilePathAnimator(DeltaTime);
}

void UShipMovementComponent::HexTilePathAnimator(float DeltaTime)
{
	if (bIsStillMoving)
	{
		if (CurrentHexTile == NextHexTile)
		{
			if (HexPathCount <= 0)
			{
				bIsStillMoving = false;
				PrintCurrentMovementRange();
			}
			else
			{
				UHexGridVisualComponent* HexGridVisual = const_cast<UHexGridVisualComponent*>(HexGridManager->GetHexGridVisual());
				HexGridVisual->HexTilePainter(CurrentHexTile, HexGridVisual->GetDefaultMaterial());
				NextHexTileCalculator();
			}
		}
		else
		{
			Timer += DeltaTime;
			if (Timer > MovementTime)
			{
				Timer = MovementTime;
				CurrentHexTile = NextHexTile;
			}
			FVector Position = FMath::Lerp(CurrentPos, TargetPos, Timer / MovementTime);
			GetOwner()->SetActorLocation(Position);
		}
	}
}

void UShipMovementComponent::NextHexTileCalculator()
{
	Timer = 0.0f;

	FHex NextHex;
	HexPath.Dequeue(NextHex);
	--HexPathCount;
	NextHexTile = HexGridManager->GetHexTile(NextHex);

	CurrentPos = CurrentHexTile->GetActorLocation();
	TargetPos = HexGridManager->GetHexTile(NextHex)->GetActorLocation();
	bIsStillMoving = true;
}

AHexTile* UShipMovementComponent::GetCurrentHexTile()
{
	return CurrentHexTile;
}

void UShipMovementComponent::SetCurrentHexTile(AHexTile* HexTile)
{
	CurrentHexTile = HexTile;
}

/*
	Function that launches a Raycast from the camera to the mouse position on the screen and checks if it collides with an AHexTile type actor. If so, it 
	calculates the path from the current position of the player to the destination.
*/
void UShipMovementComponent::MouseTargetFunction(const FVector2D& MousePosition)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !HexGridManager || bIsStillMoving)
	{
		return;
	}

	FVector WorldLocation;
	FVector WorldDirection;

	if (PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.f);

		FHitResult HitResult;
		AActor* Owner = GetOwner();
		FCollisionQueryParams TraceParams(FName(TEXT("MouseTrace")), true, Owner);
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(Owner);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

		if (bHit && HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Impactaste con: %s"), *HitResult.GetActor()->GetName());

			if (HitResult.GetActor()->IsA(AHexTile::StaticClass()))
			{
				AHexTile* TargetHexTile = Cast<AHexTile>(HitResult.GetActor());
				if (TargetHexTile)
				{
					UE_LOG(LogTemp, Warning, TEXT("¡Impactaste un objeto de la clase correcta!"));

					if (CurrentHexTile == TargetHexTile)
					{
						return;
					}
					HexGridManager->HexLinedraw(CurrentHexTile->Hex, TargetHexTile->Hex);
					NextHexTile = CurrentHexTile;
					bIsStillMoving = true;
				}
			}
		}
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);
	}
}

void UShipMovementComponent::PrintCurrentMovementRange()
{
	UHexGridVisualComponent* HexGridVisual = const_cast<UHexGridVisualComponent*>(HexGridManager->GetHexGridVisual());
	if (!HexGridVisual)
	{
		return;
	}

	int InfQ = CurrentHexTile->Hex.Q - MovementRange;
	int SupQ = CurrentHexTile->Hex.Q + MovementRange;

	//for (int q = -MovementRange; q <= MovementRange; ++q)
	//for (int q = (CurrentHexTile->Hex.Q - MovementRange); q <= (CurrentHexTile->Hex.Q + MovementRange); ++q)
	for (int q = InfQ; q <= SupQ; ++q)
	{
		int R1 = std::max(InfQ, -q - InfQ);
		int R2 = std::min(SupQ, -q + SupQ);

		for (int r = R1; r <= R2; ++r)
		{
			int s = -q - r;
			/*HexList.Add(FHex(q, r, s));

			FVector hexCoords = FVector(q, r, s);
			InstantiateCubeHexGrid(DefaultHexTile, HexToPixel(Layout, hexCoords), hexCoords);*/
			HexGridVisual->HexTilePainter(HexGridManager->GetHexTile(FHex(q, r, s)), HexGridVisual->GetRangeMaterial());
		}
	}
}