#include "Tripulacion/CrewActionNoble.h"
#include "GameFramework/Actor.h"

void UCrewActionNoble::EjecutarAccion_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("¡Nombre!"));

    if (GEngine)
    {
       GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡Acción del Noble!"));
    }
}
