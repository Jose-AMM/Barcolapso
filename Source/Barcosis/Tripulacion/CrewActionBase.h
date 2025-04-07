#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CrewActionBase.generated.h"

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class BARCOSIS_API UCrewActionBase : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void EjecutarAccion();
    virtual void EjecutarAccion_Implementation();
};