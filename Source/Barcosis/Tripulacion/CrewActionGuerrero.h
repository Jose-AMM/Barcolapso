#pragma once

#include "CoreMinimal.h"
#include "CrewActionBase.h"
#include "CrewActionGuerrero.generated.h"

UCLASS()
class BARCOSIS_API UCrewActionGuerrero : public UCrewActionBase
{
    GENERATED_BODY()

public:
    virtual void EjecutarAccion_Implementation() override;
};
