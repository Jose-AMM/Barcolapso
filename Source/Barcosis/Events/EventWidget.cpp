// EventWidget.cpp
#include "EventWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

// Define a struct to hold button click handlers
struct FOptionButtonClickHandler
{
    UEventWidget* EventWidget;
    int32 OptionIndex;

    void Execute()
    {
        if (EventWidget)
        {
            EventWidget->ProcessOptionSelection(OptionIndex);
        }
    }
};

// Array to store handlers (static to persist between function calls)
static TArray<FOptionButtonClickHandler> OptionButtonHandlers;

void UEventWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Clear the static handlers array when a new widget is constructed
    OptionButtonHandlers.Empty();

    // Hide result elements initially
    if (ResultText)
    {
        ResultText->SetVisibility(ESlateVisibility::Hidden);
    }

    if (ContinueButton)
    {
        ContinueButton->SetVisibility(ESlateVisibility::Hidden);
        ContinueButton->OnClicked.AddDynamic(this, &UEventWidget::OnContinueClicked);
    }

    bShowingResult = false;
}

void UEventWidget::InitializeEvent(UEventDataAsset* EventData)
{
    if (!EventData)
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeEvent: No event data provided"));
        return;
    }

    CurrentEvent = EventData;

    // Set initial UI
    if (EventTitleText)
    {
        EventTitleText->SetText(FText::FromString(CurrentEvent->Key));
    }

    if (EventDescriptionText)
    {
        EventDescriptionText->SetText(CurrentEvent->Text);
    }

    if (EventImage && CurrentEvent->Image)
    {
        EventImage->SetBrushFromTexture(CurrentEvent->Image);
    }

    // Show options screen
    ShowEventOptions();
}

void UEventWidget::CreateOptionButtons()
{
    if (!CurrentEvent || !OptionsContainer || !OptionButtonClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateOptionButtons: Missing required components"));
        return;
    }

    // Clear existing buttons
    OptionsContainer->ClearChildren();

    // Create a button for each option
    for (int32 i = 0; i < CurrentEvent->Options.Num(); i++)
    {
        const FEventOption& Option = CurrentEvent->Options[i];

        // Create option button widget from the template class
        UUserWidget* OptionButtonWidget = CreateWidget<UUserWidget>(this, OptionButtonClass);
        if (!OptionButtonWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create option button widget %d"), i);
            continue;
        }

        // Access the child components by name and set their properties
        if (UTextBlock* OptionTextBlock = Cast<UTextBlock>(OptionButtonWidget->GetWidgetFromName(FName("OptionText"))))
        {
            OptionTextBlock->SetText(Option.Text);
        }

        if (UTextBlock* StatTextBlock = Cast<UTextBlock>(OptionButtonWidget->GetWidgetFromName(FName("StatText"))))
        {
            StatTextBlock->SetText(FText::FromString(GetStatName(Option.Stat)));
        }

        if (UTextBlock* DifficultyTextBlock = Cast<UTextBlock>(OptionButtonWidget->GetWidgetFromName(FName("DifficultyText"))))
        {
            FString DiffString = GetDifficultyName(Option.Difficulty);
            DiffString += FString::Printf(TEXT(" (%d)"), Option.DifficultyLevel);
            DifficultyTextBlock->SetText(FText::FromString(DiffString));
        }

        // Set option image if available
        UImage* ImageWidget = Cast<UImage>(OptionButtonWidget->GetWidgetFromName(FName("OptionImage")));
        if (ImageWidget)
        {
            if (Option.Image)
            {
                ImageWidget->SetBrushFromTexture(Option.Image);
                ImageWidget->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                ImageWidget->SetVisibility(ESlateVisibility::Collapsed);
            }
        }

        // Find the button and bind the click event
        UButton* ButtonWidget = Cast<UButton>(OptionButtonWidget->GetWidgetFromName(FName("OptionButton")));
        if (ButtonWidget)
        {
            // Create a custom dynamic delegate
            int32 HandlerIndex = OptionButtonHandlers.AddDefaulted();
            OptionButtonHandlers[HandlerIndex].EventWidget = this;
            OptionButtonHandlers[HandlerIndex].OptionIndex = i;

            // Create the handler functions based on index
            switch (i)
            {
            case 0:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption0Clicked);
                break;
            case 1:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption1Clicked);
                break;
            case 2:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption2Clicked);
                break;
            case 3:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption3Clicked);
                break;
            case 4:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption4Clicked);
                break;
            case 5:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption5Clicked);
                break;
            case 6:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption6Clicked);
                break;
            case 7:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption7Clicked);
                break;
            case 8:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption8Clicked);
                break;
            case 9:
                ButtonWidget->OnClicked.AddDynamic(this, &UEventWidget::OnOption9Clicked);
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("Option index %d is out of range for predefined handlers"), i);
                break;
            }
        }

        // Add to container
        UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(OptionsContainer->AddChild(OptionButtonWidget));
        if (ButtonSlot)
        {
            ButtonSlot->SetPadding(FMargin(0, 5));
        }
    }
}

// Handler functions for option buttons (up to 10)
void UEventWidget::OnOption0Clicked() { ProcessOptionSelection(0); }
void UEventWidget::OnOption1Clicked() { ProcessOptionSelection(1); }
void UEventWidget::OnOption2Clicked() { ProcessOptionSelection(2); }
void UEventWidget::OnOption3Clicked() { ProcessOptionSelection(3); }
void UEventWidget::OnOption4Clicked() { ProcessOptionSelection(4); }
void UEventWidget::OnOption5Clicked() { ProcessOptionSelection(5); }
void UEventWidget::OnOption6Clicked() { ProcessOptionSelection(6); }
void UEventWidget::OnOption7Clicked() { ProcessOptionSelection(7); }
void UEventWidget::OnOption8Clicked() { ProcessOptionSelection(8); }
void UEventWidget::OnOption9Clicked() { ProcessOptionSelection(9); }

void UEventWidget::ProcessOptionSelection(int32 OptionIndex)
{
    if (!CurrentEvent || !CurrentEvent->Options.IsValidIndex(OptionIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("ProcessOptionSelection: Invalid option index %d"), OptionIndex);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Processing option selection: %d"), OptionIndex);

    // Get selected option
    const FEventOption& SelectedOption = CurrentEvent->Options[OptionIndex];

    // Determine success or failure
    float RandomRoll = FMath::FRand() * 100.0f; // 0-100 random float
    bool bSuccess = RandomRoll < SelectedOption.DifficultyLevel;

    UE_LOG(LogTemp, Log, TEXT("Option check: Roll %f vs Difficulty %d, Success: %s"),
        RandomRoll, SelectedOption.DifficultyLevel, bSuccess ? TEXT("YES") : TEXT("NO"));

    // Find matching result
    TArray<FEventResult> MatchingResults;
    for (const FEventResult& Result : SelectedOption.Results)
    {
        if (Result.bSuccess == bSuccess)
        {
            MatchingResults.Add(Result);
        }
    }

    // If we have matching results, pick one randomly
    if (MatchingResults.Num() > 0)
    {
        int32 ResultIndex = FMath::RandRange(0, MatchingResults.Num() - 1);
        CurrentResult = MatchingResults[ResultIndex];
    }
    else
    {
        // Fallback to empty result
        CurrentResult = FEventResult();
        CurrentResult.bSuccess = bSuccess;
        CurrentResult.Text = FText::FromString(bSuccess ?
            "You were successful, but no specific outcome is defined." :
            "You failed, but no specific outcome is defined.");
    }

    // Show the result
    ShowResult();
}

void UEventWidget::ShowEventOptions()
{
    // Show options, hide result elements
    if (OptionsContainer)
    {
        OptionsContainer->SetVisibility(ESlateVisibility::Visible);
    }

    if (ResultText)
    {
        ResultText->SetVisibility(ESlateVisibility::Hidden);
    }

    if (ContinueButton)
    {
        ContinueButton->SetVisibility(ESlateVisibility::Hidden);
    }

    bShowingResult = false;

    // Create option buttons
    CreateOptionButtons();
}

void UEventWidget::ShowResult()
{
    // Hide options, show result elements
    if (OptionsContainer)
    {
        OptionsContainer->SetVisibility(ESlateVisibility::Hidden);
    }

    if (ResultText)
    {
        ResultText->SetVisibility(ESlateVisibility::Visible);
        ResultText->SetText(CurrentResult.Text);
    }

    if (ContinueButton)
    {
        ContinueButton->SetVisibility(ESlateVisibility::Visible);
    }

    bShowingResult = true;
}

void UEventWidget::OnContinueClicked()
{
    CompleteEvent();
}

void UEventWidget::CompleteEvent()
{
    // Broadcast event completed with result codes
    OnEventCompleted.Broadcast(CurrentResult.ResultCodes);

    // Remove from parent
    RemoveFromParent();
}

FString UEventWidget::GetStatName(ECharacterStat Stat) const
{
    switch (Stat)
    {
    case ECharacterStat::Strength:
        return TEXT("Strength -> ");
    case ECharacterStat::Dexterity:
        return TEXT("Dexterity -> ");
    case ECharacterStat::Intelligence:
        return TEXT("Intelligence -> ");
    case ECharacterStat::Charisma:
        return TEXT("Charisma -> ");
    case ECharacterStat::Perception:
        return TEXT("Perception -> ");
    case ECharacterStat::None:
    default:
        return TEXT("");
    }
}

FString UEventWidget::GetDifficultyName(EDifficulty Difficulty) const
{
    switch (Difficulty)
    {
    case EDifficulty::Easy:
        return TEXT("Easy");
    case EDifficulty::Medium:
        return TEXT("Medium");
    case EDifficulty::Hard:
        return TEXT("Hard");
    case EDifficulty::Extreme:
        return TEXT("Extreme");
    case EDifficulty::None:
    default:
        return TEXT("-");
    }
}