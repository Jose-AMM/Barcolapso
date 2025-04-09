// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FollowCamera.generated.h"

/**
 * 
 */
UCLASS()
class BARCOSIS_API AFollowCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	AActor* targetToFollow = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	//bool isStatic = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float distanceToTarget = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float angleHeight = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float angleRotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float fov = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float movementSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float rotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float TransitionReachedPositionThreshold = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float TransitionMovementSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	float TransitionRotationSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Follow Settings")
	bool isDrawingDebug = false;

private:
	static AFollowCamera* Instance;

	FVector currentTargetPosition;

	FVector currentCamPosition;
	FVector targetCamPosition;

	FRotator currentCamRotation;
	FRotator targetCamRotation;

	float currentCamMovementSpeed;
	float currentCamRotationSpeed;

	bool isInTransition = false;

public:
	virtual void Tick(float DeltaTime) override;
	AFollowCamera();

	// Singleton access
	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	static AFollowCamera* GetInstance(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	void SetCameraTarget(AActor* newTarget, bool withTransition);

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	void SetCameraTargetToPlayer(bool withTransition);

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	void SetAngleRotation(float NewAngleRotation);

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	float GetAngleRotation();

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	void SetAngleHeight(float NewAngleHeight);

	UFUNCTION(BlueprintCallable, Category = "Follow Camera")
	float GetAngleHeight();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void StartCamTransition();
	void StopCamTransition();
	bool HasReachedDestination() const;
	FRotator CalcCamRotation();
	FVector CalcCamPosition();
	void DrawDebug();
};
