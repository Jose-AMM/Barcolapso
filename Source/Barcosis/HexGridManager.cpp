// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "HexTile.h"
#include <Kismet/GameplayStatics.h>
#include <ShipMovementComponent.h>

// Sets default values
AHexGridManager::AHexGridManager()
{
	LayoutPointy = FOrientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
	LayoutFlat = FOrientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

	TileHorizontalOffset = HexSize * sqrt(3);
	OddRowHorizontalOffset = TileHorizontalOffset * 0.5f;
	TileVerticalOffset = HexSize * 1.5f;
}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();

	switch (CoordsSystem)
	{
	case EHexCoordsSystem::OFFSET_SYSTEM:
		BuildOffsetHexGrid();
		break;
	case EHexCoordsSystem::CUBE_SYSTEM:
		BuildCubeHexGrid();
		break;
	default:
		break;
	}

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ShipMovement = Player->FindComponentByClass<UShipMovementComponent>();
	if (ShipMovement)
	{
		ShipMovement->HexGridManager = this;
		FHex Hex;
		Hex.Q = 0, Hex.R = 0, Hex.S = 0;
		ShipMovement->SetCurrentHexTile(GetHexTile(Hex));
	}
}

void AHexGridManager::BuildOffsetHexGrid()
{
	HexGrid2DArray.SetNumZeroed(GridWidth);
	for (int i = 0; i < HexGrid2DArray.Num(); ++i)
	{
		HexGrid2DArray[i].SetNumZeroed(GridHeight);
	}

	for (int y = 0; y < GridHeight; ++y)
	{
		for (int x = 0; x < GridWidth; ++x)
		{
			const bool OddRow = y % 2 == 1;
			const float XPos = OddRow ? (x * TileHorizontalOffset) + OddRowHorizontalOffset : x * TileHorizontalOffset;
			const float YPos = y * TileVerticalOffset;

			AHexTile* NewTile = GetWorld()->SpawnActor<AHexTile>(DefaultHexTile, FVector(FIntPoint(XPos, YPos)), FRotator::ZeroRotator);
			NewTile->TileIndex = FIntPoint(x, y);
			HexGrid2DArray[x][y] = NewTile;
		}
	}
}

/*
	Since the cubic coordinates work as its name says with 3 coordinates (q, r, s), having these the limitation of adding 0 between them,
	I calculate all the possible options depending on the number of rings that we want and then by trigonometry I calculate their real position in the world.
	So much the HexTile actor of individual form as this class saves the used coordinates for later to be able to use them in the movement,
	calculation of neighbors, pathfinding, etc.
*/
void AHexGridManager::BuildCubeHexGrid()
{
	FVector Origin = FVector(0.0, 0.0, 0.0);
	FVector2D Size = FVector2D(HexSize, HexSize);
	FLayout Layout = FLayout(LayoutPointy, Size, FIntPoint(Origin.X, Origin.Y));

	for (int q = -GridRings; q <= GridRings; ++q)
	{
		int R1 = std::max(-GridRings, -q - GridRings);
		int R2 = std::min(GridRings, -q + GridRings);

		for (int r = R1; r <= R2; ++r)
		{
			int s = -q - r;
			HexList.Add(FHex(q, r, s));

			FVector hexCoords = FVector(q, r, s);
			InstantiateCubeHexGrid(DefaultHexTile, HexToPixel(Layout, hexCoords), hexCoords);
		}
	}
}

void AHexGridManager::InstantiateCubeHexGrid(TSubclassOf<AHexTile> TileToSpawn, FVector2D Pos, FVector Hex)
{
	AHexTile* NewTile = GetWorld()->SpawnActor<AHexTile>(TileToSpawn, FVector(FIntPoint(Pos.X, Pos.Y)), FRotator::ZeroRotator);
	NewTile->Hex.Q = Hex.X;
	NewTile->Hex.R = Hex.Y;
	NewTile->Hex.S = Hex.Z;
	HexTileList.Add(NewTile);
}

AHexTile* AHexGridManager::GetHexTile(FHex Hex)
{
	AHexTile* HexTile = nullptr;

	for (int i = 0; i < HexTileList.Num(); ++i)
	{
		if (Hex.Q == HexTileList[i]->Hex.Q &&
			Hex.R == HexTileList[i]->Hex.R &&
			Hex.S == HexTileList[i]->Hex.S)
		{
			HexTile = HexTileList[i];
			break;
		}
	}
	return HexTile;
}

void AHexGridManager::HexTilePainter(AHexTile* HexTile, bool IsHexTarget)
{
	UStaticMeshComponent* MeshComponent = HexTile->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent && TargetMaterial && PathMaterial)
	{
		if (IsHexTarget)
		{
			MeshComponent->SetMaterial(0, TargetMaterial);
		}
		else
		{
			MeshComponent->SetMaterial(0, PathMaterial);
		}
	}
}

void AHexGridManager::HexTileUnpainter(AHexTile* HexTile)
{
	UStaticMeshComponent* MeshComponent = HexTile->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent && OriginalMaterial)
	{
		MeshComponent->SetMaterial(0, OriginalMaterial);
	}
}

// UNUSED!!
bool AHexGridManager::IsANeighboringHexTile(AHexTile* TargetHexTile)
{
	bool Result = false;
	AHexTile* CurrentHexTile = Player->FindComponentByClass<UShipMovementComponent>()->GetCurrentHexTile();

	for (int i = 0; i < HexDirections.Num(); ++i)
	{
		if (CurrentHexTile->Hex.Q + HexDirections[i].Q == TargetHexTile->Hex.Q &&
			CurrentHexTile->Hex.R + HexDirections[i].R == TargetHexTile->Hex.R &&
			CurrentHexTile->Hex.S + HexDirections[i].S == TargetHexTile->Hex.S)
		{
			Result = true;
			break;
		}
	}
	return Result;
}

// --------------------- HEX LIBRARY ---------------------

FVector2D AHexGridManager::HexToPixel(FLayout Layout, FVector Hex)
{
	FOrientation M = Layout.Orientation;
	float X = (M.F0 * Hex.X + M.F1 * Hex.Y) * Layout.Size.X;
	float Y = (M.F2 * Hex.X + M.F3 * Hex.Y) * Layout.Size.Y;
	return FVector2D(X + Layout.Origin.X, Y + Layout.Origin.Y);
}

float AHexGridManager::Lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
}

FFractionalHex AHexGridManager::HexLerp(FHex A, FHex B, double T)
{
	return FFractionalHex(
		Lerp(A.Q, B.Q, T),
		Lerp(A.R, B.R, T),
		Lerp(A.S, B.S, T)
	);
}

void AHexGridManager::HexLinedraw(FHex A, FHex B)
{
	if (ShipMovement)
	{
		int Distance = HexDistance(A, B);
		double Step = 1.0 / std::max(Distance, 1);
		for (int i = 0; i <= Distance; i++)
		{
			FHex Hex = HexRound(HexLerp(A, B, Step * i));
			ShipMovement->HexPath.Enqueue(Hex);
			HexTilePainter(GetHexTile(Hex), i >= Distance);
		}
		ShipMovement->SetHexPathCount(++Distance);
	}
}

int AHexGridManager::HexDistance(FHex A, FHex B)
{
	return HexLength(HexSubtract(A, B));
}

FHex AHexGridManager::HexRound(FFractionalHex H)
{
	int Q = int(round(H.Q));
	int R = int(round(H.R));
	int S = int(round(H.S));
	double Q_diff = abs(Q - H.Q);
	double R_diff = abs(R - H.R);
	double S_diff = abs(S - H.S);
	if (Q_diff > R_diff and Q_diff > S_diff) {
		Q = -R - S;
	}
	else if (R_diff > S_diff) {
		R = -Q - S;
	}
	else {
		S = -Q - R;
	}
	return FHex(Q, R, S);
}

FHex AHexGridManager::HexSubtract(FHex A, FHex B)
{
	return FHex(A.Q - B.Q, A.R - B.R, A.S - B.S);
}

int AHexGridManager::HexLength(FHex Hex)
{
	return int((abs(Hex.Q) + abs(Hex.R) + abs(Hex.S)) / 2);
}
