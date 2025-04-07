#pragma once

#include "CoreMinimal.h"
#include "CrewArquetipo.generated.h"

UENUM(BlueprintType)
enum class EArquetipo : uint8
{
    Filosofo     UMETA(DisplayName = "Filosofo"),
    Guerrero     UMETA(DisplayName = "Guerrero"),
    Noble        UMETA(DisplayName = "Noble")
};
