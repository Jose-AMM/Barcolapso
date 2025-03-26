// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "HexTile.h"

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
			const bool oddRow = y % 2 == 1;
			const float xPos = oddRow ? (x * TileHorizontalOffset) + OddRowHorizontalOffset : x * TileHorizontalOffset;
			const float yPos = y * TileVerticalOffset;

			AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(DefaultHexTile, FVector(FIntPoint(xPos, yPos)), FRotator::ZeroRotator);
			newTile->TileIndex = FIntPoint(x, y);
			HexGrid2DArray[x][y] = newTile;
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
	FVector origin = FVector(0.0, 0.0, 0.0);
	FVector2D size = FVector2D(HexSize, HexSize);
	FLayout layout = FLayout(LayoutPointy, size, FIntPoint(origin.X, origin.Y));

	for (int q = -GridRings; q <= GridRings; ++q)
	{
		int r1 = std::max(-GridRings, -q - GridRings);
		int r2 = std::min(GridRings, -q + GridRings);

		for (int r = r1; r <= r2; ++r)
		{
			int s = -q - r;
			HexList.Add(FHex(q, r, s));

			FVector hexCoords = FVector(q, r, s);
			InstantiateCubeHexGrid(DefaultHexTile, HexToPixel(layout, hexCoords), origin);
		}
	}
}

FVector2D AHexGridManager::HexToPixel(FLayout layout, FVector hex)
{
	FOrientation m = layout.Orientation;
	float x = (m.F0 * hex.X + m.F1 * hex.Y) * layout.Size.X;
	float y = (m.F2 * hex.X + m.F3 * hex.Y) * layout.Size.Y;
	return FVector2D(x + layout.Origin.X, y + layout.Origin.Y);
}

void AHexGridManager::InstantiateCubeHexGrid(TSubclassOf<AHexTile> tileToSpawn, FVector2D pos, FVector hex)
{
	AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(tileToSpawn, FVector(FIntPoint(pos.X, pos.Y)), FRotator::ZeroRotator);
	newTile->Hex.Q = hex.X;
	newTile->Hex.R = hex.Y;
	newTile->Hex.S = hex.Z;
}
