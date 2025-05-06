// Fill out your copyright notice in the Description page of Project Settings.


#include "Tripulacion/CrewEnemy.h"
#include "Components/SphereComponent.h"
#include "Tripulacion/TripulacionManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACrewEnemy::ACrewEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    RootComponent = Trigger;
    Trigger->InitSphereRadius(200.f);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ACrewEnemy::BeginPlay()
{
	Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACrewEnemy::OnOverlapBegin);
	
}

// Called every frame
void ACrewEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrewEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (OtherActor == PlayerPawn)
    {
        if (UTripulacionManager* Manager = PlayerPawn->FindComponentByClass<UTripulacionManager>())
        {
            if (!Manager->TieneTripulantes())
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Silver, TEXT("NO MERECES LA PENA... VUELVE CUANDO TENGAS TRIPULANTES"));
                }

                return;
            }

            Manager->RecibirAtaque(EArquetipo::Guerrero, FuerzaAtaque);
            Destroy();
        }
    }
}


