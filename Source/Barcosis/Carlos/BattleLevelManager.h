// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/TimerHandle.h"
#include "Templates/SubclassOf.h"
#include "Containers/Array.h"
#include "UObject/ObjectPtr.h"
#include "BattleLevelManager.generated.h"

class UStaticMesh;
class UWorld;
class ULevelStreaming;
class AActor;
class ACrewCharacter;

/**
 * 
 */
UCLASS()
class BARCOSIS_API UBattleLevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	// Soft pointer to the default level map asset to stream load. Set this in the Editor details panel.
	UPROPERTY(EditAnywhere, Category = "Battle Level", meta = (AllowedClasses = "World"))
	TSoftObjectPtr<UWorld> BattleLevelAsset;

	UPROPERTY(Transient)
	TSoftObjectPtr<UWorld> CurrentBattleLevelAsset;

	// Default spawn location if needed (LoadStreamLevel doesn't use it directly)
	UPROPERTY(EditAnywhere, Category = "Battle Level")
	FVector BattleInstanceSpawnLocation = FVector(-1150.0f, -7480.0f, 50.0f);

	// Classes to spawn for each enemy type.
	UPROPERTY(Transient)
	TSubclassOf<AActor> EnemyAClassToSpawn;

	UPROPERTY(Transient)
	TSubclassOf<AActor> EnemyBClassToSpawn;

	UPROPERTY(Transient)
	TSubclassOf<AActor> EnemyCClassToSpawn;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> MeshFloor;

	// Timer handle for delaying configuration slightly after level load
	FTimerHandle ConfigureTimerHandle;

	// Keep track of actors spawned during the battle for cleanup. Using TObjectPtr for safety.
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> SpawnedBattleActors;

public:
	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End USubsystem Interface

	/**
	 * Loads the specified battle level additively (asynchronously) using a Soft Object Pointer,
	 * and once loaded (with a frame delay), spawns actors based on the provided classes
	 * at designated BattleSpawnLocations and configures the battle floor.
	 *
	 * @param LevelToLoadAsset Optional override Soft Pointer for the level asset to load. If invalid, uses BattleLevelAsset member.
	 * @param InEnemyAClass Class of the actor to spawn at the "EnemyA" location.
	 * @param InEnemyBClass Class of the actor to spawn at the "EnemyB" location.
	 * @param InEnemyCClass Class of the actor to spawn at the "EnemyC" location.
	 * @param CustomMeshFloor Mesh for the actor tagged 'BattleFloor'.
	 */
	UFUNCTION(BlueprintCallable, Category = "Battle Level", meta = (DisplayName = "Load And Configure Battle Level Async"))
	void LoadAndConfigureBattleLevel(
		TSoftObjectPtr<UWorld> LevelToLoadAsset,
		TSubclassOf<AActor> InEnemyAClass = nullptr, // Default to nullptr if not provided
		TSubclassOf<AActor> InEnemyBClass = nullptr,
		TSubclassOf<AActor> InEnemyCClass = nullptr,
		UStaticMesh* CustomMeshFloor = nullptr);

	/**
	 * Unloads the currently active battle level (or the specified one) and destroys actors spawned during that battle.
	 * Uses internal GetWorld(). (Still uses blocking unload for now).
	 * @param LevelToUnloadAsset Optional override Soft Pointer for the level asset to unload. If invalid, uses the currently stored asset path.
	 */
	UFUNCTION(BlueprintCallable, Category = "Battle Level", meta = (DisplayName = "Unload Battle Level"))
	void UnloadBattleLevel(TSoftObjectPtr<UWorld> LevelToUnloadAsset = nullptr);

	/**
	 * Finds BattleSpawnLocations, spawns enemies of the configured classes at those locations.
	 * Finds the BattleFloor actor, sets its mesh, positions the player, and sets the camera target.
	 * Called by DeferredConfigureBattle after a short delay.
	 */
	void ConfigureBattleScene();

	/**
	 * Delegate function called when the streamed level has finished loading. Sets up a timer for deferred configuration.
	 */
	UFUNCTION() // Needs to be UFUNCTION() to be bound to the delegate
		void OnBattleLevelStreamLoaded();

	/**
	 * Function called by the timer after OnBattleLevelStreamLoaded fires, to actually perform configuration.
	 */
	void DeferredConfigureBattle(); // Renamed for clarity

	/**
	 * Destroys all actors stored in the SpawnedBattleActors array.
	 */
	void CleanupSpawnedBattleActors();
};
