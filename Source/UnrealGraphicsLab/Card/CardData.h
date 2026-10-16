#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CardTypes.h"
#include "CardData.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct FCardAttack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TArray<ECardEnergyType> Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (MultiLine = true))
	FText Description;
};

UCLASS(BlueprintType)
class UNREALGRAPHICSLAB_API UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Stage = INVTEXT("BASIC");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (ClampMin = 10))
	int32 HP = 70;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	ECardEnergyType Type = ECardEnergyType::Colorless;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TArray<FCardAttack> Attacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card|Weakness")
	bool bHasWeakness = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card|Weakness", meta = (EditCondition = "bHasWeakness"))
	ECardEnergyType Weakness = ECardEnergyType::Fighting;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card|Resistance")
	bool bHasResistance = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card|Resistance", meta = (EditCondition = "bHasResistance"))
	ECardEnergyType Resistance = ECardEnergyType::Colorless;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (ClampMin = 0, ClampMax = 3))
	int32 RetreatCost = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card|Visuals")
	TObjectPtr<UMaterialInterface> CardMaterial;
};