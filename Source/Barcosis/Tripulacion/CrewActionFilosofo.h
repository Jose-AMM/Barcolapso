#pragma once

#include "CoreMinimal.h"
#include "CrewActionBase.h"
#include "CrewActionFilosofo.generated.h"

UCLASS()
class BARCOSIS_API UCrewActionFilosofo : public UCrewActionBase
{
    GENERATED_BODY()

public:
    virtual void EjecutarAccion_Implementation() override;
};
