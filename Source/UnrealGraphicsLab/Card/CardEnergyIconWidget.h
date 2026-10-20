#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardTypes.h"
#include "CardEnergyIconWidget.generated.h"

class UImage;
class UTexture2D;

USTRUCT(BlueprintType)
struct FCardEnergyStyle
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FLinearColor CircleColor = FLinearColor::White;
};

UCLASS(Abstract)
class UNREALGRAPHICSLAB_API UCardEnergyIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCardEnergyIconWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetEnergyType(ECardEnergyType InEnergyType);
	
	UFUNCTION(BlueprintPure, Category = "Card")
	ECardEnergyType GetEnergyType() const { return EnergyType; }
	
protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	ECardEnergyType EnergyType = ECardEnergyType::Colorless;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Card")
	TMap<ECardEnergyType, FCardEnergyStyle> Styles;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CircleImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TypeIcon;
};