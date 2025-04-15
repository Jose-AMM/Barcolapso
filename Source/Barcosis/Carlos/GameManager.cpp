// Fill out your copyright notice in the Description page of Project Settings.

#include "Carlos/GameManager.h"
#include "Carlos/FollowCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Level.h"
#include "Engine/LevelStreaming.h"
#include "UObject/SoftObjectPath.h"
#include "TimerManager.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("GameManager initialized. Default Battle Level Asset: %s"), *BattleLevelAsset.ToString());
}

void UGameManager::Deinitialize()
{
	// Clear timer if it's active when subsystem is destroyed
	if (UWorld* World = GetWorld()) {
		World->GetTimerManager().ClearTimer(ConfigureTimerHandle);
	}

	MeshA = nullptr;
	MeshB = nullptr;
	MeshC = nullptr;
	MeshFloor = nullptr;
	Super::Deinitialize();
}

void UGameManager::LoadAndConfigureBattleLevel(
	TSoftObjectPtr<UWorld> LevelToLoadAsset,
	UStaticMesh* CustomMeshA,
	UStaticMesh* CustomMeshB,
	UStaticMesh* CustomMeshC,
	UStaticMesh* CustomMeshFloor)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadAndConfigureBattleLevel: Failed - GetWorld() returned null."));
		return;
	}

	// Clear any previous timer just in case
	World->GetTimerManager().ClearTimer(ConfigureTimerHandle);

	// Store the level asset reference for later unloading
	CurrentBattleLevelAsset = LevelToLoadAsset.IsValid() ? LevelToLoadAsset : BattleLevelAsset;


	// --- 1. Store desired meshes ---
	MeshA = CustomMeshA;
	MeshB = CustomMeshB;
	MeshC = CustomMeshC;
	MeshFloor = CustomMeshFloor;
	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Storing target meshes."));

	// --- 2. Determine Level Asset Pointer ---
	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Attempting to load level asset ASYNCHRONOUSLY: %s in World: %s"), *LevelToLoadAsset.ToString(), *World->GetName());

	// --- 3. Load Level (Asynchronous) ---
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, LevelToLoadAsset, true, false, FLatentActionInfo());

	// --- 4. Bind Callback to Level Loaded Delegate ---
	const FName LevelFName = LevelToLoadAsset.GetLongPackageFName();
	if (LevelFName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("LoadAndConfigureBattleLevel: Could not derive valid FName from Soft Pointer: %s. Cannot bind delegate."), *LevelToLoadAsset.ToString());
		return;
	}

	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(World, LevelFName);

	if (StreamingLevel)
	{
		UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Found StreamingLevel object for %s. Binding OnLevelLoaded delegate."), *LevelFName.ToString());
		StreamingLevel->OnLevelLoaded.AddDynamic(this, &UGameManager::OnBattleLevelStreamLoaded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadAndConfigureBattleLevel: Could not find StreamingLevel object for %s immediately after LoadStreamLevel call. Delegate not bound."), *LevelFName.ToString());
	}

	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Asynchronous load initiated for %s. Configuration will occur on delegate callback (with delay)."), *LevelToLoadAsset.ToString());
}

// Delegate called when level load finishes
void UGameManager::OnBattleLevelStreamLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("OnBattleLevelStreamLoaded: Delegate fired. Setting timer for deferred configuration."));

	UWorld* World = GetWorld();
	if (World)
	{
		// Instead of calling ConfigureMeshesInWorld directly, set a short timer.
		// 0.01f is usually enough to push execution to the next frame.
		World->GetTimerManager().SetTimer(
			ConfigureTimerHandle,         // Timer handle
			this,                         // Object to call function on
			&UGameManager::DeferredConfigureMeshes, // Function to call
			0.01f,                        // Delay in seconds
			false                         // Don't loop
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnBattleLevelStreamLoaded: GetWorld() returned null, cannot set timer."));
	}

	// Optional: Unbind here if you might load the *exact same level instance* again
	// and don't want this delegate firing multiple times for future loads.
	// Find the ULevelStreaming object again (potentially tricky if name isn't stored)
	// and call OnLevelLoaded.RemoveDynamic(...)
}

// Called by the timer after a short delay
void UGameManager::DeferredConfigureMeshes()
{
	UE_LOG(LogTemp, Log, TEXT("DeferredConfigureMeshes: Timer finished. Proceeding with mesh configuration."));
	ConfigureMeshesInWorld();
}


// ConfigureMeshesInWorld remains the same
void UGameManager::ConfigureMeshesInWorld()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ConfigureMeshesInWorld: Failed - GetWorld() returned null."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Searching for tagged actors in World: %s"), *World->GetName());

	int FoundCount = 0;
	bool bMeshASet = false;
	bool bMeshBSet = false;
	bool bMeshCSet = false;
	bool bMeshFloorSet = false;

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;
		if (!Actor) continue;

		UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			FoundCount++;
			if (!bMeshASet && Actor->ActorHasTag(FName("Spawn1")) && MeshA) {
				MeshComp->SetStaticMesh(MeshA);
				bMeshASet = true;
				UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Set MeshA on actor '%s' with tag 'Spawn1'"), *Actor->GetName());
			}
			else if (!bMeshBSet && Actor->ActorHasTag(FName("Spawn2")) && MeshB) {
				MeshComp->SetStaticMesh(MeshB);
				bMeshBSet = true;
				UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Set MeshB on actor '%s' with tag 'Spawn2'"), *Actor->GetName());
			}
			else if (!bMeshCSet && Actor->ActorHasTag(FName("Spawn3")) && MeshC) {
				MeshComp->SetStaticMesh(MeshC);
				bMeshCSet = true;
				UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Set MeshC on actor '%s' with tag 'Spawn3'"), *Actor->GetName());
			}
			else if (!bMeshFloorSet && Actor->ActorHasTag(FName("BattleFloor")) && MeshFloor) {
				MeshComp->SetStaticMesh(MeshFloor);
				bMeshFloorSet = true;
				UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Set MeshFloor on actor '%s' with tag 'BattleFloor'"), *Actor->GetName());
			}
		}

		if (Actor->ActorHasTag(FName("BattleFloor"))) {
			// Set Camera to Target the Battle Floor
			AFollowCamera* MainCamera = AFollowCamera::GetInstance(GetWorld());
			MainCamera->SetCameraTarget(Actor, false);

			// Teleport the Player to the Battle Floor position
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController && PlayerController->GetPawn())
			{
				FVector TargetLocation = Actor->GetActorLocation();

				TargetLocation.Z += 100.0f;

				// Teleport the player's pawn to the target location
				PlayerController->GetPawn()->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

				UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Teleported player to battle floor position."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: Could not teleport player - PlayerController or Pawn is null."));
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Iteration complete. Found %d actors with StaticMeshComponents."), FoundCount);

	if (!bMeshASet) UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: MeshA (Tag 'Spawn1') not found or set."));
	if (!bMeshBSet) UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: MeshB (Tag 'Spawn2') not found or set."));
	if (!bMeshCSet) UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: MeshC (Tag 'Spawn3') not found or set."));
	if (!bMeshFloorSet) UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: MeshFloor (Tag 'BattleFloor') not found or set."));
}

void UGameManager::UnloadBattleLevel(TSoftObjectPtr<UWorld> LevelToUnloadAsset)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UnloadBattleLevel: Failed to get World reference."));
		return;
	}

	// Use the provided level asset, or fall back to the stored one, or finally to the default
	const TSoftObjectPtr<UWorld>& LevelPtr = LevelToUnloadAsset.IsValid() ?
		LevelToUnloadAsset : (CurrentBattleLevelAsset.IsValid() ?
			CurrentBattleLevelAsset : BattleLevelAsset);

	if (!LevelPtr.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UnloadBattleLevel: No valid level asset soft pointer provided or configured."));
		return;
	}

	const FName LevelFName = LevelPtr.GetLongPackageFName();

	if (LevelFName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("UnloadBattleLevel: Could not derive valid FName from Soft Pointer: %s"), *LevelPtr.ToString());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: Attempting to unload level by name derived from Soft Pointer: %s (%s)"), *LevelFName.ToString(), *LevelPtr.ToString());

	UGameplayStatics::UnloadStreamLevel(this, LevelFName, FLatentActionInfo(), true); // Still blocking

	// Reset the current battle level reference
	if (CurrentBattleLevelAsset == LevelPtr)
	{
		CurrentBattleLevelAsset = nullptr;
	}

	// Set Camera to Target the Battle Floor
	AFollowCamera* MainCamera = AFollowCamera::GetInstance(GetWorld());
	MainCamera->SetCameraTargetToPlayer(false);

	// Teleport the Player to the Battle Floor position
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController && PlayerController->GetPawn())
	{
		FVector TargetLocation = FVector(830.0f, -140.0f, 0.0f);
		// Optional: Add offset to prevent player from spawning inside floor
		TargetLocation.Z += 100.0f; // Adjust this value as needed

		// Teleport the player's pawn to the target location
		PlayerController->GetPawn()->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

		UE_LOG(LogTemp, Log, TEXT("ConfigureMeshesInWorld: Teleported player to battle floor position."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfigureMeshesInWorld: Could not teleport player - PlayerController or Pawn is null."));
	}

	UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: UnloadStreamLevel (blocking) called for %s."), *LevelFName.ToString());
}