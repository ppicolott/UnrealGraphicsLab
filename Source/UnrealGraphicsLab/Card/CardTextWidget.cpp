#include "CardTextWidget.h"

#include "CardData.h"
#include "CardEnergyIconWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Materials/MaterialInterface.h"

void UCardTextWidget::SetCardData(UCardData* InCardData)
{
	CardData = InCardData;
	Refresh();
}

void UCardTextWidget::Refresh()
{
	if (!CardData)
	{
		return;
	}
	
	ApplyUIMaterial();
	
	// Header
	TitleText->SetText(CardData->Name);
	StageText->SetText(CardData->Stage);
	HPText->SetText(FText::AsNumber(CardData->HP));
	WBP_EnergyIcon_Card->SetEnergyType(CardData->Type);
	
	// Attacks
	struct FAttackWidgets
	{
		UWidget* Box;
		UTextBlock* Name;
		UTextBlock* Damage;
		UTextBlock* Description;
		TArray<UCardEnergyIconWidget*> CostIcons;
	};
	
	const FAttackWidgets AttackWidgets[] =
	{
		{ AttackBox1, AttackName1, AttackDamage1, AttackText1, { WBP_EnergyIcon_1_1, WBP_EnergyIcon_1_2, WBP_EnergyIcon_1_3 } },
		{ AttackBox2, AttackName2, AttackDamage2, AttackText2, { WBP_EnergyIcon_2_1, WBP_EnergyIcon_2_2, WBP_EnergyIcon_2_3 } },
	};
	
	for (int32 i = 0; i < UE_ARRAY_COUNT(AttackWidgets); i++)
	{
		const FAttackWidgets& Widgets = AttackWidgets[i];
		
		// A card with fewer attacks hides the unused boxes
		if (!CardData->Attacks.IsValidIndex(i))
		{
			Widgets.Box->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}
		
		const FCardAttack& Attack = CardData->Attacks[i];
		Widgets.Box->SetVisibility(ESlateVisibility::Visible);
		Widgets.Name->SetText(Attack.Name);
		Widgets.Damage->SetText(Attack.Damage);
		Widgets.Description->SetText(Attack.Description);
		SetIconRow(Widgets.CostIcons, Attack.Cost);
	}
	
	// Weakness
	const ESlateVisibility WeaknessVisibility = CardData->bHasWeakness ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	WBP_EnergyIcon_Weakness->SetEnergyType(CardData->Weakness);
	WBP_EnergyIcon_Weakness->SetVisibility(WeaknessVisibility);
	WeaknessText->SetVisibility(WeaknessVisibility);
	
	// Resistance
	const ESlateVisibility ResistanceVisibility = CardData->bHasResistance ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	WBP_EnergyIcon_Resistance->SetEnergyType(CardData->Resistance);
	WBP_EnergyIcon_Resistance->SetVisibility(ResistanceVisibility);
	ResistanceText->SetVisibility(ResistanceVisibility);
	
	// Retreat cost is always Colorless: One icon per point of cost (up to 3)
	TArray<ECardEnergyType> RetreatTypes;
	RetreatTypes.Init(ECardEnergyType::Colorless, CardData->RetreatCost);
	SetIconRow({ WBP_EnergyIcon_Retreat_1, WBP_EnergyIcon_Retreat_2, WBP_EnergyIcon_Retreat_3 }, RetreatTypes);
}

void UCardTextWidget::SetIconRow(TConstArrayView<UCardEnergyIconWidget*> Icons, TConstArrayView<ECardEnergyType> Types)
{
	for (int32 i = 0; i < Icons.Num(); i++)
	{
		if (Types.IsValidIndex(i))
		{
			Icons[i]->SetEnergyType(Types[i]);
			Icons[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Icons[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UCardTextWidget::ApplyUIMaterial()
{
	// Remember the designer's metal the first time, so an empty UIMaterial can restore it
	if (!DefaultUIMaterial)
	{
		DefaultUIMaterial = StageBadge->Background.GetResourceObject();
	}
	
	UObject* Material = CardData->UIMaterial ? CardData->UIMaterial.Get() : DefaultUIMaterial.Get();
	UMaterialInterface* UIMaterial = Cast<UMaterialInterface>(Material);
	if (!UIMaterial)
	{
		return;
	}
	
	StageBadge->SetBrushFromMaterial(UIMaterial);
	for (UImage* Bar : { WeaknessBar.Get(), ResistanceBar.Get(), RetreatBar.Get() })
	{
		Bar->SetBrushFromMaterial(UIMaterial);
	}
}

void UCardTextWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Refresh();
}