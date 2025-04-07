#include "Tripulacion/CrewCharacter.h"
#include "Tripulacion/CrewActionGuerrero.h"
#include "Tripulacion/CrewActionFilosofo.h"

#include "Tripulacion/CrewActionNoble.h"



#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ACrewCharacter::ACrewCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshVisual"));
    RootComponent = MeshVisual;

    SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
    SphereTrigger->InitSphereRadius(150.f);
    SphereTrigger->SetupAttachment(RootComponent);

    SphereTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    SphereTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereTrigger->SetGenerateOverlapEvents(true);
}

void ACrewCharacter::BeginPlay()
{
    Super::BeginPlay();

    switch (Arquetipo)
    {
    case EArquetipo::Guerrero:
        AccionEspecial = NewObject<UCrewActionGuerrero>(this);
        break;
    case EArquetipo::Filosofo:
        AccionEspecial = NewObject<UCrewActionFilosofo>(this);
        break;
    case EArquetipo::Noble:
        AccionEspecial = NewObject<UCrewActionNoble>(this);
        break;
    default:
        break;
    }

    SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACrewCharacter::OnOverlapBegin);
}

void ACrewCharacter::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (GEngine)
    {
        FString TextoStats = FString::Printf(
            TEXT("%s (%s)\nLogos: %d | Thymos: %d | Ethos: %d\nAisthesis: %d | Akousma: %d | Eukrasia: %d | Charis: %d"),
            *NombreVisible,
            *UEnum::GetValueAsString(Arquetipo),
            Stats.Logos,
            Stats.Thymos,
            Stats.Ethos,
            Stats.Aisthesis,
            Stats.Akousma,
            Stats.Eukrasia,
            Stats.Charis
        );

        GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Green, TextoStats);
    }

    if (AccionEspecial)
    {
        AccionEspecial->EjecutarAccion();
    }
}

void ACrewCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
