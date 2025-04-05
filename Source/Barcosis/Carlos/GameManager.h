// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/TimerHandle.h" // Needed for FTimerHandle
#include "GameManager.generated.h"

class UStaticMesh;
class UWorld;
class ULevelStreaming; // Forward declaration

/**
 * Manages loading and configuring battle levels additively using Soft Object Pointers (Asynchronously with frame delay).
 */
UCLASS()
class BARCOSIS_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	// Soft pointer to the default level map asset to stream load. Set this in the Editor details panel.
	UPROPERTY(EditAnywhere, Category = "Battle Level", meta = (AllowedClasses = "World"))
	TSoftObjectPtr<UWorld> BattleLevelAsset;

	// Default spawn location if needed (LoadStreamLevel doesn't use it directly)
	UPROPERTY(EditAnywhere, Category = "Battle Level")
	FVector BattleInstanceSpawnLocation = FVector(-1150.0f, -7480.0f, 50.0f);

	// Current mesh assets for the battle, set via LoadAndConfigureBattleLevel
	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> MeshA;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> MeshB;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> MeshC;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> MeshFloor;

	// Timer handle for delaying configuration slightly after level load
	FTimerHandle ConfigureTimerHandle;

public:
	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End USubsystem Interface

	/**
	 * Loads the specified battle level additively (asynchronously) using a Soft Object Pointer
	 * and configures tagged meshes within it once loading is complete (with a one-frame delay).
	 * Uses internal GetWorld() for context.
	 * @param LevelToLoadAsset Optional override Soft Pointer for the level asset to load. If invalid, uses BattleLevelAsset member.
	 * @param CustomMeshA Mesh for actors tagged 'Spawn1'.
	 * @param CustomMeshB Mesh for actors tagged 'Spawn2'.
	 * @param CustomMeshC Mesh for actors tagged 'Spawn3'.
	 * @param CustomMeshFloor Mesh for actors tagged 'BattleFloor'.
	 */
	UFUNCTION(BlueprintCallable, Category = "Battle Level", meta = (DisplayName = "Load And Configure Battle Level Async By Soft Pointer"))
	void LoadAndConfigureBattleLevel(
		TSoftObjectPtr<UWorld> LevelToLoadAsset,
		UStaticMesh* CustomMeshA = nullptr,
		UStaticMesh* CustomMeshB = nullptr,
		UStaticMesh* CustomMeshC = nullptr,
		UStaticMesh* CustomMeshFloor = nullptr);

	/**
	 * Unloads the specified battle level identified by a Soft Object Pointer. Uses internal GetWorld(). (Still uses blocking unload for now).
	 * @param LevelToUnloadAsset Optional override Soft Pointer for the level asset to unload. If invalid, uses BattleLevelAsset member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Battle Level", meta = (DisplayName = "Unload Battle Level By Soft Pointer"))
	void UnloadBattleLevel(TSoftObjectPtr<UWorld> LevelToUnloadAsset);

private:
	/**
	 * Finds actors with specific tags in the current world (via GetWorld()) and sets their static meshes.
	 * Called by DeferredConfigureMeshes after a short delay.
	 */
	void ConfigureMeshesInWorld();

	/**
	 * Delegate function called when the streamed level has finished loading. Sets up a timer for deferred configuration.
	 */
	UFUNCTION() // Needs to be UFUNCTION() to be bound to the delegate
		void OnBattleLevelStreamLoaded();

	/**
	 * Function called by the timer after OnBattleLevelStreamLoaded fires, to actually perform configuration.
	 */
	void DeferredConfigureMeshes(); // Not a UFUNCTION
};