
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Events/EventDataAsset.h"
#include "Events/EventWidget.h"
#include "GM_EventTest.generated.h"

UCLASS()
class BARCOSIS_API AGM_EventTest : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGM_EventTest();

    // The widget class to use for event displays
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_AEvents")
    TSubclassOf<UEventWidget> EventWidgetClass;

    // Function to trigger an event - can be called from level blueprint
    UFUNCTION(BlueprintCallable, Category = "_AEvents")
    void StartEvent(UEventDataAsset* EventData);

protected:
    // Called when an event is completed
    UFUNCTION()
    void OnEventCompleted(const TArray<FString>& ResultCodes);

    // Process individual result codes
    UFUNCTION(BlueprintImplementableEvent, Category = "_AEvents")
    void ProcessResultCode(const FString& ResultCode);
};