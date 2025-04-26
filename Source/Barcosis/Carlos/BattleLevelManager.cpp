// Fill out your copyright notice in the Description page of Project Settings.


#include "Carlos/BattleLevelManager.h"
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
#include "Carlos/BattleSpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"


void UBattleLevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("BattleLevelManager initialized. Default Battle Level Asset: %s"), *BattleLevelAsset.ToString());
}

void UBattleLevelManager::Deinitialize()
{
	// Clear timer if it's active when subsystem is destroyed
	if (UWorld* World = GetWorld()) {
		World->GetTimerManager().ClearTimer(ConfigureTimerHandle);
	}

	CleanupSpawnedBattleActors();

	EnemyAClassToSpawn = nullptr;
	EnemyBClassToSpawn = nullptr;
	EnemyCClassToSpawn = nullptr;
	MeshFloor = nullptr;

	CurrentBattleLevelAsset = nullptr;

	Super::Deinitialize();
}

void UBattleLevelManager::LoadAndConfigureBattleLevel(
	TSoftObjectPtr<UWorld> LevelToLoadAsset,
	TSubclassOf<AActor> InEnemyAClass,
	TSubclassOf<AActor> InEnemyBClass,
	TSubclassOf<AActor> InEnemyCClass,
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
	CleanupSpawnedBattleActors();

	CurrentBattleLevelAsset = LevelToLoadAsset.IsValid() ? LevelToLoadAsset : BattleLevelAsset;

	EnemyAClassToSpawn = InEnemyAClass;
	EnemyBClassToSpawn = InEnemyBClass;
	EnemyCClassToSpawn = InEnemyCClass;
	MeshFloor = CustomMeshFloor;
	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Storing configuration. EnemyA Class: %s, EnemyB Class: %s, EnemyC Class: %s"),
		*GetNameSafe(InEnemyAClass), *GetNameSafe(InEnemyBClass), *GetNameSafe(InEnemyCClass));

	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Attempting to load level asset ASYNCHRONOUSLY: %s in World: %s"), *LevelToLoadAsset.ToString(), *World->GetName());

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, LevelToLoadAsset, true, false, FLatentActionInfo());

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

		StreamingLevel->OnLevelLoaded.RemoveDynamic(this, &UBattleLevelManager::OnBattleLevelStreamLoaded);
		StreamingLevel->OnLevelLoaded.AddDynamic(this, &UBattleLevelManager::OnBattleLevelStreamLoaded);
	}
	else
	{
		// This can happen if the level load fails very early or if called immediately after unload.
		// Consider adding a fallback mechanism or more robust check if this proves problematic.
		UE_LOG(LogTemp, Warning, TEXT("LoadAndConfigureBattleLevel: Could not find StreamingLevel object for %s immediately after LoadStreamLevel call. Delegate binding might be delayed or fail if load fails."), *LevelFName.ToString());
	}

	UE_LOG(LogTemp, Log, TEXT("LoadAndConfigureBattleLevel: Asynchronous load initiated for %s. Configuration will occur on delegate callback (with delay)."), *CurrentBattleLevelAsset.ToString());
}

// Delegate called when level load finishes
void UBattleLevelManager::OnBattleLevelStreamLoaded()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("OnBattleLevelStreamLoaded: GetWorld() returned null, cannot set timer for configuration."));
		return;
	}

	if (CurrentBattleLevelAsset.IsValid()) {
		const FName LevelFName = FName(*CurrentBattleLevelAsset.GetLongPackageName());
		ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(World, LevelFName);
		if (StreamingLevel) {
			UE_LOG(LogTemp, Log, TEXT("OnBattleLevelStreamLoaded: Unbinding delegate for %s."), *LevelFName.ToString());
			StreamingLevel->OnLevelLoaded.RemoveDynamic(this, &UBattleLevelManager::OnBattleLevelStreamLoaded);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnBattleLevelStreamLoaded: CurrentBattleLevelAsset is invalid, cannot unbind delegate precisely."));
	}


	UE_LOG(LogTemp, Log, TEXT("OnBattleLevelStreamLoaded: Delegate fired. Setting timer for deferred configuration."));

	// Set a short timer to delay configuration until the next frame or slightly after.
	// This helps ensure all level actors are fully initialized.
	World->GetTimerManager().SetTimer(
		ConfigureTimerHandle,
		this,
		&UBattleLevelManager::DeferredConfigureBattle,
		0.02f, // Slightly increased delay can sometimes help
		false
	);
}

void UBattleLevelManager::DeferredConfigureBattle()
{
	UE_LOG(LogTemp, Log, TEXT("DeferredConfigureBattle: Timer finished. Proceeding with battle scene configuration."));
	ConfigureBattleScene();
}


void UBattleLevelManager::ConfigureBattleScene()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ConfigureBattleScene: Failed - GetWorld() returned null."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Searching for BattleSpawnLocations and BattleFloor in World: %s"), *World->GetName());

	// --- 1. Find BattleSpawnLocation Actors and Spawn Enemies ---
	TArray<AActor*> FoundSpawnLocations;
	UGameplayStatics::GetAllActorsOfClass(World, ABattleSpawnLocation::StaticClass(), FoundSpawnLocations);

	UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Found %d BattleSpawnLocation actors."), FoundSpawnLocations.Num());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (AActor* Actor : FoundSpawnLocations)
	{
		ABattleSpawnLocation* SpawnLocation = Cast<ABattleSpawnLocation>(Actor);
		if (!SpawnLocation)
		{
			continue;
		}

		const FVector Location = SpawnLocation->GetActorLocation();
		const FRotator Rotation = SpawnLocation->GetActorRotation();
		const FString& SpawnName = SpawnLocation->SpawnLocationName;

		UE_LOG(LogTemp, Verbose, TEXT("ConfigureBattleScene: Checking Spawn Location '%s' with name '%s'."), *SpawnLocation->GetName(), *SpawnName);

		AActor* SpawnedActor = nullptr;

		if (SpawnName == TEXT("EnemyA"))
		{
			if (EnemyAClassToSpawn)
			{
				SpawnedActor = World->SpawnActor<AActor>(EnemyAClassToSpawn, Location, Rotation, SpawnParams);
				if (SpawnedActor)
				{
					UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Spawned %s at '%s' (%s)"), *GetNameSafe(EnemyAClassToSpawn), *SpawnLocation->GetName(), *Location.ToString());
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("ConfigureBattleScene: Failed to spawn actor of class %s at '%s'."), *GetNameSafe(EnemyAClassToSpawn), *SpawnLocation->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found 'EnemyA' spawn point '%s', but GameManager::EnemyAClassToSpawn is null."), *SpawnLocation->GetName());
			}
		}
		else if (SpawnName == TEXT("EnemyB"))
		{
			if (EnemyBClassToSpawn)
			{
				SpawnedActor = World->SpawnActor<AActor>(EnemyBClassToSpawn, Location, Rotation, SpawnParams);
				if (SpawnedActor)
				{
					UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Spawned %s at '%s' (%s)"), *GetNameSafe(EnemyBClassToSpawn), *SpawnLocation->GetName(), *Location.ToString());
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("ConfigureBattleScene: Failed to spawn actor of class %s at '%s'."), *GetNameSafe(EnemyBClassToSpawn), *SpawnLocation->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found 'EnemyB' spawn point '%s', but GameManager::EnemyBClassToSpawn is null."), *SpawnLocation->GetName());
			}
		}
		else if (SpawnName == TEXT("EnemyC"))
		{
			if (EnemyCClassToSpawn)
			{
				SpawnedActor = World->SpawnActor<AActor>(EnemyCClassToSpawn, Location, Rotation, SpawnParams);
				if (SpawnedActor)
				{
					UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Spawned %s at '%s' (%s)"), *GetNameSafe(EnemyCClassToSpawn), *SpawnLocation->GetName(), *Location.ToString());
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("ConfigureBattleScene: Failed to spawn actor of class %s at '%s'."), *GetNameSafe(EnemyCClassToSpawn), *SpawnLocation->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found 'EnemyC' spawn point '%s', but GameManager::EnemyCClassToSpawn is null."), *SpawnLocation->GetName());
			}
		}

		if (SpawnedActor)
		{
			SpawnedBattleActors.Add(SpawnedActor);
		}

	}

	// --- 2. Find BattleFloor Actor and Configure Camera/Player/Mesh ---
	TArray<AActor*> FoundFloorActors;
	UGameplayStatics::GetAllActorsWithTag(World, FName("BattleFloor"), FoundFloorActors);
	bool bFloorProcessed = false;

	if (FoundFloorActors.Num() > 0)
	{
		AActor* FloorActor = FoundFloorActors[0]; // Assume the first one found is the correct one
		if (FloorActor)
		{
			bFloorProcessed = true;
			UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Found BattleFloor actor '%s' with tag 'BattleFloor'."), *FloorActor->GetName());

			// Set Camera Target
			AFollowCamera* MainCamera = AFollowCamera::GetInstance(GetWorld());
			if (MainCamera)
			{
				MainCamera->SetCameraTarget(FloorActor, false);

				MainCamera->distanceToTarget = 2000;

				UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Set camera target to '%s'."), *FloorActor->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Could not find AFollowCamera instance to set target."));
			}

			// Teleport Player
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				APawn* PlayerPawn = PlayerController->GetPawn();
				if (PlayerPawn)
				{
					FVector TargetLocation = FloorActor->GetActorLocation();
					TargetLocation.Z += 100.0f; // Offset Z to avoid spawning inside floor
					PlayerPawn->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
					UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Teleported player to BattleFloor location %s."), *TargetLocation.ToString());
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Could not get Player Pawn to teleport."));
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Could not get PlayerController to teleport player."));
			}

			// Set Floor Mesh
			UStaticMeshComponent* FloorMeshComp = FloorActor->FindComponentByClass<UStaticMeshComponent>();
			if (FloorMeshComp && MeshFloor)
			{
				FloorMeshComp->SetStaticMesh(MeshFloor);
				UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Set MeshFloor on actor '%s'."), *FloorActor->GetName());
			}
			else if (!MeshFloor)
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found BattleFloor actor '%s' but GameManager::MeshFloor is null, cannot set mesh."), *FloorActor->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found BattleFloor actor '%s' but it has no StaticMeshComponent."), *FloorActor->GetName());
			}
		}

		if (FoundFloorActors.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: Found %d actors with tag 'BattleFloor'. Using the first one found: '%s'."), FoundFloorActors.Num(), *GetNameSafe(FloorActor));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: No actor found with tag 'BattleFloor'. Player/Camera/FloorMesh not configured relative to floor."));
	}

	// --- 3. Final Logging ---
	UE_LOG(LogTemp, Log, TEXT("ConfigureBattleScene: Configuration process complete. Spawned %d actors."), SpawnedBattleActors.Num());
	//if (!EnemyAClassToSpawn && FoundSpawnLocations.FindByPredicate([](AActor* A) { return Cast<ABattleSpawnLocation>(A) && Cast<ABattleSpawnLocation>(A)->SpawnLocationName == TEXT("EnemyA"); })) UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: 'EnemyA' spawn location found, but no class was provided to spawn."));
	// Add similar checks for B and C if desired
	if (!bFloorProcessed) UE_LOG(LogTemp, Warning, TEXT("ConfigureBattleScene: BattleFloor processing skipped (actor with tag 'BattleFloor' not found)."));

}

void UBattleLevelManager::UnloadBattleLevel(TSoftObjectPtr<UWorld> LevelToUnloadAsset)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UnloadBattleLevel: Failed to get World reference."));
		return;
	}

	CleanupSpawnedBattleActors();

	// Determine which level asset to unload
	const TSoftObjectPtr<UWorld>& LevelPtr = LevelToUnloadAsset.IsValid() ?
		LevelToUnloadAsset : CurrentBattleLevelAsset;

	if (!LevelPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnloadBattleLevel: No valid level asset soft pointer provided or stored from previous load. Cannot determine level to unload."));
	}
	else
	{
		const FName LevelFName = FName(*LevelPtr.GetLongPackageName());

		if (LevelFName.IsNone())
		{
			UE_LOG(LogTemp, Error, TEXT("UnloadBattleLevel: Could not derive valid FName from Soft Pointer: %s"), *LevelPtr.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: Attempting to unload level: %s (%s)"), *LevelFName.ToString(), *LevelPtr.ToString());

			FLatentActionInfo LatentInfo;
			UGameplayStatics::UnloadStreamLevel(this, LevelFName, LatentInfo, true);

			UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: UnloadStreamLevel call completed for %s."), *LevelFName.ToString());
		}
	}

	CurrentBattleLevelAsset = nullptr;
	EnemyAClassToSpawn = nullptr;
	EnemyBClassToSpawn = nullptr;
	EnemyCClassToSpawn = nullptr;
	MeshFloor = nullptr;


	// --- Reset Player and Camera ---
	AFollowCamera* MainCamera = AFollowCamera::GetInstance(GetWorld());
	if (MainCamera)
	{
		MainCamera->SetCameraTargetToPlayer(false);
		MainCamera->distanceToTarget = 1000;

		UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: Reset camera target to player."));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UnloadBattleLevel: Could not find AFollowCamera instance to reset target."));
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController && PlayerController->GetPawn())
	{
		FVector DefaultWorldLocation = FVector(830.0f, -140.0f, 100.0f);
		PlayerController->GetPawn()->SetActorLocation(DefaultWorldLocation, false, nullptr, ETeleportType::TeleportPhysics);
		UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: Teleported player back to default location %s."), *DefaultWorldLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UnloadBattleLevel: Could not teleport player back - PlayerController or Pawn is null."));
	}

	UE_LOG(LogTemp, Log, TEXT("UnloadBattleLevel: Process finished."));
}


void UBattleLevelManager::CleanupSpawnedBattleActors()
{
	if (SpawnedBattleActors.Num() > 0) {
		UE_LOG(LogTemp, Log, TEXT("CleanupSpawnedBattleActors: Destroying %d previously spawned battle actors."), SpawnedBattleActors.Num());
		for (TObjectPtr<AActor>& ActorPtr : SpawnedBattleActors)
		{
			if (ActorPtr)
			{
				ActorPtr->Destroy();
			}
		}
		SpawnedBattleActors.Empty();
	}
	else {
		UE_LOG(LogTemp, Verbose, TEXT("CleanupSpawnedBattleActors: No actors in SpawnedBattleActors array to clean up."));
	}
}
