#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputRecruitComponent.generated.h"

class ACrewCharacter;
class UTripulacionManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BARCOSIS_API UInputRecruitComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UInputRecruitComponent();
    
    static UInputRecruitComponent* ReclutacionActiva;

    ACrewCharacter* Crew;

    UTripulacionManager* TripulacionManager;

    void Reclutar();

    void Rechazar();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

    virtual void BeginPlay() override;
   
};
