#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrewStats.h"
#include "TripulacionManager.h"
#include "CrewArquetipo.h"
#include "CrewActionBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CrewCharacter.generated.h"

UCLASS()
class BARCOSIS_API ACrewCharacter : public AActor
{
    GENERATED_BODY()

public:
    ACrewCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* SphereTrigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    FString NombreVisible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    EArquetipo Arquetipo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCrewStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    int32 Reputacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    float Phobos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    bool bEsCapitan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tripulante")
    EPasivaCapitan PasivaCapitan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
    UCrewActionBase* AccionEspecial;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    virtual void Tick(float DeltaTime) override;
};
