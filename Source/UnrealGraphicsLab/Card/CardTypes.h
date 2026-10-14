#pragma once

#include "CoreMinimal.h"
#include "CardTypes.generated.h"

UENUM(BlueprintType)
enum class ECardEnergyType : uint8
{
	Grass,
	Fire,
	Water,
	Lightning,
	Fighting,
	Darkness,
	Colorless
};