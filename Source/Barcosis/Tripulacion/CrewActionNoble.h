#pragma once

#include "CoreMinimal.h"
#include "CrewActionBase.h"
#include "CrewActionNoble.generated.h"

UCLASS()
class BARCOSIS_API UCrewActionNoble : public UCrewActionBase
{
    GENERATED_BODY()

public:
    virtual void EjecutarAccion_Implementation() override;
};
