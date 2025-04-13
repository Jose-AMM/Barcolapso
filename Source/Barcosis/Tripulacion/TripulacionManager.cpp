#include "Tripulacion/TripulacionManager.h"
#include "Tripulacion/CrewCharacter.h"
#include "TripulanteInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UTripulacionManager::UTripulacionManager()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTripulacionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && PC->WasInputKeyJustPressed(EKeys::L))
    {
        MostrarTripulacion();
    }
}

void UTripulacionManager::AnadirTripulante(ACrewCharacter* Nuevo)
{
    if (!Nuevo) return;

    FTripulanteInfo Info;
    Info.NombreVisible = Nuevo->NombreVisible;
    Info.Arquetipo = Nuevo->Arquetipo;
    Info.Stats = Nuevo->Stats;
    Info.bEsCapitan = Nuevo->bEsCapitan;

    Tripulantes.Add(Info);

    if (Info.bEsCapitan)
    {
        CapitanIndex = Tripulantes.Num() - 1;
    }
}

void UTripulacionManager::MostrarTripulacion() const
{
    if (Tripulantes.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("No tiene ningun tripulante"));
        return;
    }

    for (int32 i = 0; i < Tripulantes.Num(); ++i)
    {
        const FTripulanteInfo& P = Tripulantes[i];

        FString Info = FString::Printf(TEXT("Tripulante: %s (%s)%s\nLogos: %d | Thymos: %d | Ethos: %d | Aisthesis: %d"),
            *P.NombreVisible,
            *UEnum::GetValueAsString(P.Arquetipo),
            (i == CapitanIndex) ? TEXT(" [CAPITAN]") : TEXT(""),
            P.Stats.Logos,
            P.Stats.Thymos,
            P.Stats.Ethos,
            P.Stats.Aisthesis
        );

        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow, Info);
    }

    // CARACTERISTICAS DEL BARCO
    int32 MaxLogos = ObtenerAtributoMaximo("Logos");
    int32 MaxThymos = ObtenerAtributoMaximo("Thymos");
    int32 MaxEthos = ObtenerAtributoMaximo("Ethos");
    int32 MaxAisthesis = ObtenerAtributoMaximo("Aisthesis");

    int32 TotalLogos = ObtenerAtributoTotal("Logos");
    int32 TotalThymos = ObtenerAtributoTotal("Thymos");
    int32 TotalEthos = ObtenerAtributoTotal("Ethos");
    int32 TotalAisthesis = ObtenerAtributoTotal("Aisthesis");

    FString GeneralInfo = FString::Printf(TEXT("----ATRIBUTOS TRIPULACION ---\n")\
        TEXT("Mayor Logos: %d | Total Logos: %d\n")\
        TEXT("Mayor Thymos: %d | Total Thymos: %d\n")\
        TEXT("Mayor Ethos: %d | Total Ethos: %d\n")\
        TEXT("Mayor Aisthesis: %d | Total Aisthesis: %d"),
        MaxLogos, TotalLogos,
        MaxThymos, TotalThymos,
        MaxEthos, TotalEthos,
        MaxAisthesis, TotalAisthesis
    );

    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, GeneralInfo);

}

int32 UTripulacionManager::ObtenerAtributoMaximo(FName NombreAtributo) const
{
    int32 Max = 0;
    for (const auto& P : Tripulantes)
    {
        int32 Valor = P.Stats.ObtenerValorPorNombre(NombreAtributo);
        Max = FMath::Max(Max, Valor);
    }
    return Max;
}

int32 UTripulacionManager::ObtenerAtributoTotal(FName NombreAtributo, float MultiplicadorCapitan) const
{
    int32 Total = 0;
    for (int32 i = 0; i < Tripulantes.Num(); ++i)
    {
        const auto& P = Tripulantes[i];
        int32 Valor = P.Stats.ObtenerValorPorNombre(NombreAtributo);
        Total += (i == CapitanIndex) ? Valor * MultiplicadorCapitan : Valor;
    }
    return Total;
}

