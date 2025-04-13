#pragma once

#include "CoreMinimal.h"
#include "CrewArquetipo.h"
#include "CrewStats.h"
#include "TripulanteInfo.generated.h"

USTRUCT(BlueprintType)
struct FTripulanteInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NombreVisible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EArquetipo Arquetipo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEsCapitan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCrewStats Stats;
};
