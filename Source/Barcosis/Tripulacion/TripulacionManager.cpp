#include "Tripulacion/TripulacionManager.h"
#include "Tripulacion/CrewCharacter.h"

void UTripulacionManager::AnadirTripulante(ACrewCharacter* Nuevo)
{
    if (!Nuevo) return;

    Tripulantes.Add(Nuevo);

    if (Nuevo->bEsCapitan)
    {
        if (Capitan && Capitan != Nuevo)
        {
            Capitan->bEsCapitan = false;
        }

        Capitan = Nuevo;
    }
}


int32 UTripulacionManager::ObtenerMaximoAtributo(FName NombreAtributo) const
{
    int32 Max = 0;
    for (const auto& P : Tripulantes)
    {
        if (!P) continue;

        int32 Valor = P->Stats.ObtenerValorPorNombre(NombreAtributo);
        if (Valor > Max)
        {
            Max = Valor;
        }
    }
    return Max;
}

int32 UTripulacionManager::ObtenerSumaAtributo(FName NombreAtributo, float MultiplicadorCapitan) const
{
    int32 Total = 0;
    for (const auto& P : Tripulantes)
    {
        if (!P) continue;

        int32 Valor = P->Stats.ObtenerValorPorNombre(NombreAtributo);
        if (P == Capitan)
            Total += Valor * MultiplicadorCapitan;
        else
            Total += Valor;
    }
    return Total;
}

void UTripulacionManager::AplicarPasivaCapitan()
{
    if (!Capitan) return;

    switch (Capitan->PasivaCapitan)
    {
    case EPasivaCapitan::Estratega:
        UE_LOG(LogTemp, Warning, TEXT("Estratega activada."));
        break;
    case EPasivaCapitan::Suerte:
        UE_LOG(LogTemp, Warning, TEXT("Suerte activada."));
        break;
    default:
        break;
    }
}

void UTripulacionManager::EvaluarPhobos()
{
    for (const auto& P : Tripulantes)
    {
        if (P && P->Phobos > 80)
        {
            UE_LOG(LogTemp, Error, TEXT("%s hay miedo."), *P->NombreVisible);
        }
    }
}

float UTripulacionManager::CalcularReputacionTotal() const
{
    float Reputacion = 0.f;
    for (const auto& P : Tripulantes)
    {
        if (P)
        {
            Reputacion += P->Reputacion + P->Stats.Logos + P->Stats.Charis;
        }
    }
    return Reputacion;
}
