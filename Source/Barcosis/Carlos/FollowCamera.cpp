// Fill out your copyright notice in the Description page of Project Settings.


#include "Carlos/FollowCamera.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include <Carlos/GameManager.h>
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerInput.h"

AFollowCamera* AFollowCamera::Instance = nullptr;

AFollowCamera::AFollowCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();

	if (Instance != nullptr && Instance != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Duplicate FollowCameraActor detected. Destroying duplicate."));
		Destroy();
		return;
	}

	Instance = this;

	// reset transition parameters just in case
	StopCamTransition();
}

void AFollowCamera::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Instance == this)
	{
		Instance = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

AFollowCamera* AFollowCamera::GetInstance(UWorld* World)
{
	if (Instance == nullptr)
	{
		for (TActorIterator<AFollowCamera> ActorItr(World); ActorItr; ++ActorItr)
		{
			Instance = *ActorItr;
			break;
		}
	}
	return Instance;
}

void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (targetToFollow == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera: There is no target!"));
		return;
	}

	//if (isStatic) {
	//	currentCamMovementSpeed = 0.0f;
	//}

	currentTargetPosition = targetToFollow->GetActorLocation();

	FRotator smoothTargetCamRotation = CalcCamRotation();
	FVector smoothTargetCamPosition = CalcCamPosition();

	SetActorLocationAndRotation(smoothTargetCamPosition, smoothTargetCamRotation);

	GetCameraComponent()->SetFieldOfView(fov);

	if (isInTransition && HasReachedDestination())
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera reached the target position."));

		StopCamTransition();
	}

	if (isDrawingDebug) {
		DrawDebug();
	}

	// debug keys

	/*
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R))
	{
		isDrawingDebug = !isDrawingDebug;
	}

	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Q) && !isInTransition)
	{
		SetCameraTargetToPlayer(true);
	}

	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::M))
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			if (UUIManager* UIMgr = GI->GetSubsystem<UUIManager>())
			{
				if (!UIMgr->isInMainMenu) {
					UIMgr->ShowMainMenu();
				}
			}
		}
	}

	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::N))
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			if (UGameManager* GameMgr = GI->GetSubsystem<UGameManager>())
			{
				if (UUIManager* UIMgr = GI->GetSubsystem<UUIManager>())
				{
					if (!UIMgr->isInPauseMenu) {
						GameMgr->PauseGame();
					}
				}
			}
		}
	}
	*/
}

/// <summary>
/// Changes the camera target to the received new one, with transition or not
/// </summary>
/// <param name="newTarget"></param>
void AFollowCamera::SetCameraTarget(AActor* newTarget, bool withTransition = true)
{
	if (newTarget == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera: New target is null!"));
		return;
	}

	targetToFollow = newTarget;

	if (withTransition) {
		StartCamTransition();
	}
	// TODO: else a teleport to the final position?
}

/// <summary>
/// Sets the camera target to the player
/// </summary>
void AFollowCamera::SetCameraTargetToPlayer(bool withTransition = true)
{
	AActor* playerActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (playerActor != nullptr) {
		SetCameraTarget(GetWorld()->GetFirstPlayerController()->GetPawn(), withTransition);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera: Couldn't find Player!"));
	}
}

void AFollowCamera::SetAngleRotation(float NewAngleRotation)
{
	angleRotation = NewAngleRotation;
}

/// <summary>
/// Starts a camera transition
/// </summary>
void AFollowCamera::StartCamTransition()
{
	isInTransition = true;
	currentCamMovementSpeed = TransitionMovementSpeed;
	currentCamRotationSpeed = TransitionRotationSpeed;
}

/// <summary>
/// Stops current camera transition
/// </summary>
void AFollowCamera::StopCamTransition()
{
	isInTransition = false;
	currentCamMovementSpeed = movementSpeed;
	currentCamRotationSpeed = rotationSpeed;
}

/// <summary>
/// Checks if the camera is near its target position
/// </summary>
/// <returns></returns>
bool AFollowCamera::HasReachedDestination() const
{
	float distance = FVector::Dist(GetActorLocation(), targetCamPosition);

	//UE_LOG(LogTemp, Warning, TEXT("Distance of transition: %f"), distance);

	return distance < TransitionReachedPositionThreshold;
}

/// <summary>
/// Calculates and returns the desired camera rotator to look at target.
/// </summary>
/// <returns></returns>
FRotator AFollowCamera::CalcCamRotation()
{
	FVector cameraLocation = GetActorLocation();

	targetCamRotation = FRotationMatrix::MakeFromX(currentTargetPosition - cameraLocation).Rotator();

	currentCamRotation = GetActorRotation();

	float deltaTime = GetWorld()->GetDeltaSeconds();

	FRotator finalCamRotation = FMath::RInterpTo(currentCamRotation, targetCamRotation, deltaTime, currentCamRotationSpeed);

	return finalCamRotation;
}

/// <summary>
/// Calculates and returns the desired camera position to follow the target.
/// </summary>
/// <returns></returns>
FVector AFollowCamera::CalcCamPosition()
{
	currentCamPosition = GetActorLocation();

	float radHeight = FMath::DegreesToRadians(angleHeight);
	float radRotation = FMath::DegreesToRadians(angleRotation);

	float polarCoordinatesOffsetX = -distanceToTarget * FMath::Cos(radHeight) * FMath::Cos(radRotation);
	float polarCoordinatesOffsetY = -distanceToTarget * FMath::Cos(radHeight) * FMath::Sin(radRotation);
	float polarCoordinatesOffsetZ = distanceToTarget * FMath::Sin(radHeight);
	FVector polarCoordinatesOffset = FVector(polarCoordinatesOffsetX, polarCoordinatesOffsetY, polarCoordinatesOffsetZ);

	targetCamPosition = currentTargetPosition + polarCoordinatesOffset;

	float deltaTime = GetWorld()->GetDeltaSeconds();

	FVector finalCamPosition = FMath::Lerp(currentCamPosition, targetCamPosition, currentCamMovementSpeed * deltaTime);

	return finalCamPosition;
}

/// <summary>
/// Draws a representation of the current camera settings for debug and testing purposes.
/// </summary>
void AFollowCamera::DrawDebug()
{
	DrawDebugSphere(GetWorld(), currentTargetPosition, distanceToTarget, 90, FColor(255, 0, 0), false, 0.0f, 0, 1.0f);

	FVector currentPosition = GetActorLocation();

	DrawDebugLine(GetWorld(), currentTargetPosition, currentPosition, FColor(0, 0, 255), false, 0.0f, 0, 5.0f);

	DrawDebugSphere(GetWorld(), currentPosition, 20.0f, 12, FColor(0, 0, 255), false, 0.0f, 0, 20.0f);
}

/// <summary>
/// Camera angle rotation getter
/// </summary>
float AFollowCamera::GetAngleRotation()
{
	return angleRotation;
}

void AFollowCamera::SetAngleHeight(float NewAngleHeight)
{
	angleHeight = NewAngleHeight;
}

float AFollowCamera::GetAngleHeight()
{
	return angleHeight;
}
