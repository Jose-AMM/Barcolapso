#include "Tripulacion/CrewActionFilosofo.h"
#include "GameFramework/Actor.h"

void UCrewActionFilosofo::EjecutarAccion_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("¡Filosofo!"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡Acción del Filosofo!"));
    }
}
