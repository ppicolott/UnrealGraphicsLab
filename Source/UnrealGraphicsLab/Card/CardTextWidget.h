#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardTypes.h"
#include "CardTextWidget.generated.h"

class UCardData;
class UCardEnergyIconWidget;
class UTextBlock;
class UWidget;
class UBorder;
class UImage;
class UMaterialInterface;

UCLASS(Abstract)
class UNREALGRAPHICSLAB_API UCardTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardData(UCardData* InCardData);
	
	UFUNCTION(BlueprintPure, Category = "Card")
	UCardData* GetCardData() const { return CardData; }
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void Refresh();

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UCardData> CardData;
	
	// Widgets below are bound by name to the ones in WBP_CardText
	
	// Header
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Card;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> StageBadge;
	
	// Attack 1
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> AttackBox1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackName1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackDamage1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackText1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_1_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_1_2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_1_3;
	
	// Attack 2
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> AttackBox2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackName2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackDamage2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackText2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_2_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_2_2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_2_3;
	
	// Footer
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Weakness;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaknessText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Resistance;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResistanceText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Retreat_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Retreat_2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCardEnergyIconWidget> WBP_EnergyIcon_Retreat_3;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> WeaknessBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ResistanceBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> RetreatBar;
	
private:
	static void SetIconRow(TConstArrayView<UCardEnergyIconWidget*> Icons, TConstArrayView<ECardEnergyType> Types);
	
	// The designer's UI metal, restored when the card data has no UIMaterial
	UPROPERTY(Transient)
	TObjectPtr<UObject> DefaultUIMaterial;

	void ApplyUIMaterial();
};