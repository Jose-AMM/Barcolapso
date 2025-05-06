#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrewStats.h"
#include "TripulanteInfo.h"
#include "TripulacionManager.generated.h"

class ACrewCharacter;

UENUM(BlueprintType)
enum class ERasgoPasivo : uint8
{
    Ninguno,
    OjoDeAguila,
    Duelista,
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

    UTripulacionManager();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void AnadirTripulante(ACrewCharacter* Nuevo);
    void MostrarTripulacion() const;

    //
    UFUNCTION(BlueprintCallable)
    void RecibirAtaque(EArquetipo TipoEnemigo, int32 FuerzaEnemiga);
    UFUNCTION(BlueprintCallable)
    void EliminarTripulante(int32 Index);
    UFUNCTION(BlueprintCallable)
    bool TieneTripulantes() const;
    UFUNCTION(BlueprintCallable)
    void Derrota();
    UFUNCTION(BlueprintCallable)
    void Victoria(AActor* OtherActor);
    void AplicarPenalizacionPorMuerte(EArquetipo TipoEliminado);



private:
    TArray<FTripulanteInfo> Tripulantes;
    int32 CapitanIndex = -1;

    int32 ObtenerAtributoMaximo(FName NombreAtributo) const;
    int32 ObtenerAtributoTotal(FName NombreAtributo, float MultiplicadorCapitan = 2.0f) const;

};
