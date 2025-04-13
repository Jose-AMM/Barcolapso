#include "Tripulacion/CrewActionGuerrero.h"
#include "GameFramework/Actor.h"

void UCrewActionGuerrero::EjecutarAccion_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("¡Guerrero ataca!"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡Acción del Guerrero!"));
    }
}
