#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrewStats.h"
#include "TripulacionManager.generated.h"

class ACrewCharacter;

UENUM(BlueprintType)
enum class ERasgoPasivo : uint8
{
    Ninguno,
    OjoDeAguila,
    Intimidante,
    Heroe,
    Asceta,
    Estoico,
    Semidios
};

UENUM(BlueprintType)
enum class EPasivaCapitan : uint8
{
    Ninguna,
    Estratega,
    Orador,
    Capataz,
    Negociador,
    Comerciante,
    Suerte
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BARCOSIS_API UTripulacionManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ACrewCharacter*> Tripulantes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ACrewCharacter* Capitan;

    UFUNCTION(BlueprintCallable)
    void AnadirTripulante(ACrewCharacter* Nuevo);

    UFUNCTION(BlueprintCallable)
    int32 ObtenerMaximoAtributo(FName NombreAtributo) const;

    UFUNCTION(BlueprintCallable)
    int32 ObtenerSumaAtributo(FName NombreAtributo, float MultiplicadorCapitan = 2.0f) const;

    UFUNCTION(BlueprintCallable)
    void AplicarPasivaCapitan();

    UFUNCTION(BlueprintCallable)
    void EvaluarPhobos();

    UFUNCTION(BlueprintCallable)
    float CalcularReputacionTotal() const;
};