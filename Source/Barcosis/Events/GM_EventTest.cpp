#include "GM_EventTest.h"
#include "Kismet/GameplayStatics.h"

AGM_EventTest::AGM_EventTest()
{
    // Initialize default values
    PrimaryActorTick.bCanEverTick = false;
}

void AGM_EventTest::StartEvent(UEventDataAsset* EventData)
{
    if (!EventData)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartEvent: No event data provided"));
        return;
    }

    if (!EventWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartEvent: No event widget class set in GameMode"));
        return;
    }

    // Get player controller
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartEvent: Unable to get player controller"));
        return;
    }

    // Create the event widget
    UEventWidget* EventWidget = CreateWidget<UEventWidget>(PC, EventWidgetClass);
    if (!EventWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartEvent: Failed to create event widget"));
        return;
    }

    // Set up the widget with event data
    EventWidget->InitializeEvent(EventData);

    // Bind to event completion
    EventWidget->OnEventCompleted.AddDynamic(this, &AGM_EventTest::OnEventCompleted);

    // Add to viewport
    EventWidget->AddToViewport();

    // Set input mode to UI
    PC->SetInputMode(FInputModeUIOnly());
    PC->bShowMouseCursor = true;
}

void AGM_EventTest::OnEventCompleted(const TArray<FString>& ResultCodes)
{
    // Reset input mode
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    // Process each result code
    for (const FString& ResultCode : ResultCodes)
    {
        ProcessResultCode(ResultCode);
    }
}