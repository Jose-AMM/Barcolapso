// EventWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EventDataAsset.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "EventWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventCompleted, const TArray<FString>&, ResultCodes);

/**
 * Widget that displays an event, its options, and handles the entire event flow
 */
UCLASS()
class BARCOSIS_API UEventWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
     * Initialize the widget with an event data asset
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void InitializeEvent(UEventDataAsset* EventData);

    /**
     * Called when an event has been completed (after a result is shown)
     */
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnEventCompleted OnEventCompleted;

    /**
     * Process an option selection - public to be called from option handlers
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void ProcessOptionSelection(int32 OptionIndex);

protected:
    virtual void NativeConstruct() override;

    /**
     * The event data we're displaying
     */
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    UEventDataAsset* CurrentEvent;

    /**
     * The current result being shown
     */
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    FEventResult CurrentResult;

    /**
     * Whether we're showing the event options or a result
     */
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    bool bShowingResult;

    /**
     * UI Elements - Bind these in the widget blueprint
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* EventTitleText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* EventDescriptionText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* EventImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* OptionsContainer;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ResultText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ContinueButton;

    /**
     * Template for option buttons
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Event")
    TSubclassOf<UUserWidget> OptionButtonClass;

    /**
     * Handler for when the continue button is clicked
     */
    UFUNCTION()
    void OnContinueClicked();

    /**
     * Creates and displays all the option buttons
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void CreateOptionButtons();

    /**
     * Option button click handlers (one for each possible option)
     */
    UFUNCTION()
    void OnOption0Clicked();

    UFUNCTION()
    void OnOption1Clicked();

    UFUNCTION()
    void OnOption2Clicked();

    UFUNCTION()
    void OnOption3Clicked();

    UFUNCTION()
    void OnOption4Clicked();

    UFUNCTION()
    void OnOption5Clicked();

    UFUNCTION()
    void OnOption6Clicked();

    UFUNCTION()
    void OnOption7Clicked();

    UFUNCTION()
    void OnOption8Clicked();

    UFUNCTION()
    void OnOption9Clicked();

    /**
     * Shows the event options screen
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void ShowEventOptions();

    /**
     * Shows the result screen
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void ShowResult();

    /**
     * Closes the event widget
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    void CompleteEvent();

    /**
     * Gets the stat name as a string
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    FString GetStatName(ECharacterStat Stat) const;

    /**
     * Gets the difficulty name as a string
     */
    UFUNCTION(BlueprintCallable, Category = "Event")
    FString GetDifficultyName(EDifficulty Difficulty) const;
};