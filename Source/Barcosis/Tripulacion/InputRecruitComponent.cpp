#include "Tripulacion/InputRecruitComponent.h"
#include "Tripulacion/CrewCharacter.h"
#include "Tripulacion/TripulacionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UInputRecruitComponent* UInputRecruitComponent::ReclutacionActiva = nullptr;

UInputRecruitComponent::UInputRecruitComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInputRecruitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    // R PARA RECLUTAR
    if (ReclutacionActiva == this && PC->WasInputKeyJustPressed(EKeys::R))
    {
        Reclutar();
    }

    // T PARA RECHAZAR
    if (ReclutacionActiva == this && PC->WasInputKeyJustPressed(EKeys::T))
    {
        Rechazar();
    }
}

void UInputRecruitComponent::BeginPlay()
{
    Super::BeginPlay();

    Crew = Cast<ACrewCharacter>(GetOwner());

    // BUSCAR EL TRIPULACION MANAGER DEL CHARACTER
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        TripulacionManager = PlayerPawn->FindComponentByClass<UTripulacionManager>();
        if (!TripulacionManager)
        {
            UE_LOG(LogTemp, Error, TEXT("TripulacionManager no encontrado."));
        }
    }
}

void UInputRecruitComponent::Reclutar()
{
    if (ReclutacionActiva && ReclutacionActiva->TripulacionManager && ReclutacionActiva->Crew)
    {
        ReclutacionActiva->TripulacionManager->AnadirTripulante(ReclutacionActiva->Crew);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Tripulante ANADIDO"));
        }

        ReclutacionActiva->Crew->Destroy();
        ReclutacionActiva = nullptr;
    }
}

void UInputRecruitComponent::Rechazar()
{
    if (ReclutacionActiva && ReclutacionActiva->Crew)
    {
        ReclutacionActiva->Crew->Destroy();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Tripulante RECHAZADO"));
        }

        ReclutacionActiva = nullptr;
    }
}
