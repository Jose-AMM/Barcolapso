#pragma once

#include "CoreMinimal.h"
#include "CrewStats.generated.h"

USTRUCT(BlueprintType)
struct FCrewStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Logos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Thymos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Ethos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Aisthesis;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Akousma;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Eukrasia;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Charis;

    int32 ObtenerValorPorNombre(FName Nombre) const
    {
        if (Nombre == "Logos") return Logos;
        if (Nombre == "Thymos") return Thymos;
        if (Nombre == "Ethos") return Ethos;
        if (Nombre == "Aisthesis") return Aisthesis;
        if (Nombre == "Akousma") return Akousma;
        if (Nombre == "Eukrasia") return Eukrasia;
        if (Nombre == "Charis") return Charis;
        return 0;
    }

};