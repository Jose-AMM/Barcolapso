// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EventDataAsset.generated.h"

/**
 * Types of events
 */
UENUM(BlueprintType)
enum class EEventType : uint8
{
    None    UMETA(DisplayName = "None"),
    Port    UMETA(DisplayName = "Port"),
    Sea     UMETA(DisplayName = "Sea"),
    Island  UMETA(DisplayName = "Island"),
    Shore   UMETA(DisplayName = "Shore"),
    Ship    UMETA(DisplayName = "Ship")
    // Add more as needed
};

/**
 * Character stats that can be tested in events
 */
UENUM(BlueprintType)
enum class ECharacterStat : uint8
{
    None        UMETA(DisplayName = "None"),
    Strength    UMETA(DisplayName = "Strength"),
    Dexterity   UMETA(DisplayName = "Dexterity"),
    Intelligence UMETA(DisplayName = "Intelligence"),
    Charisma    UMETA(DisplayName = "Charisma"),
    Perception  UMETA(DisplayName = "Perception")
    // Add more as needed
};

/**
 * Difficulty levels for event challenges
 */
UENUM(BlueprintType)
enum class EDifficulty : uint8
{
    None    UMETA(DisplayName = "None"),
    Easy    UMETA(DisplayName = "Easy"),
    Medium  UMETA(DisplayName = "Medium"),
    Hard    UMETA(DisplayName = "Hard"),
    Extreme UMETA(DisplayName = "Extreme")
    // Add more as needed
};

/**
 * Represents a possible result of an event option
 */
USTRUCT(BlueprintType)
struct BARCOSIS_API FEventResult
{
    GENERATED_BODY()

    // Whether this result represents success
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bSuccess;

    // Terror effect (x = fear, y = madness or whatever vector2 represents)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100"))
    FVector2D Terror;

    // Description text for this result
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText Text;

    // Result codes (for triggering game effects)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> ResultCodes;
};

/**
 * Represents an option for an event
 */
USTRUCT(BlueprintType)
struct BARCOSIS_API FEventOption
{
    GENERATED_BODY()

    // The character stat required for this option
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECharacterStat Stat;

    // Difficulty level enum
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EDifficulty Difficulty;

    // Numeric difficulty level
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100"))
    int32 DifficultyLevel;

    // Modifier to the player's stat when checking success
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 StatModifier;

    // Description text for this option
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText Text;

    // Image to display with this option
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Image;

    // Terror effect for choosing this option (regardless of success/failure)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100"))
    FVector2D Terror;

    // Possible results from selecting this option
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FEventResult> Results;
};

/**
 * Data asset containing information about a single event with multiple options
 */
UCLASS(BlueprintType)
class BARCOSIS_API UEventDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // Unique identifier for this event
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    FString Key;

    // Types of event (can be multiple)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    TArray<EEventType> Types;

    // Event image
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    UTexture2D* Image;

    // Description text for the event
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (MultiLine = true))
    FText Text;

    // Options available for this event
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    TArray<FEventOption> Options;
};

class BARCOSIS_API EventDataAsset
{
public:
	EventDataAsset();
	~EventDataAsset();
};
