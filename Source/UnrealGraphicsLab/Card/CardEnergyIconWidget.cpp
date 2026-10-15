#include "CardEnergyIconWidget.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

UCardEnergyIconWidget::UCardEnergyIconWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject)) return;
	
	auto AddStyle = [this](ECardEnergyType Type, const TCHAR* IconPath, const TCHAR* ColorHex)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(IconPath);
		FCardEnergyStyle Style;
		Style.Icon = IconFinder.Object;
		Style.CircleColor = FLinearColor(FColor::FromHex(ColorHex));
		Styles.Add(Type, Style);
	};
	
	AddStyle(ECardEnergyType::Grass, TEXT("/Game/ThirdParty/GameIcons/sprout.sprout"), TEXT("5DA83A"));
	AddStyle(ECardEnergyType::Fire, TEXT("/Game/ThirdParty/GameIcons/fire.fire"), TEXT("E2462F"));
	AddStyle(ECardEnergyType::Water, TEXT("/Game/ThirdParty/GameIcons/water-drop.water-drop"), TEXT("3A8ED6"));
	AddStyle(ECardEnergyType::Lightning, TEXT("/Game/ThirdParty/GameIcons/electric.electric"), TEXT("F0C02C"));
	AddStyle(ECardEnergyType::Fighting, TEXT("/Game/ThirdParty/GameIcons/fist.fist"), TEXT("B5562B"));
	AddStyle(ECardEnergyType::Darkness, TEXT("/Game/ThirdParty/GameIcons/moon.moon"), TEXT("2F3B48"));
	AddStyle(ECardEnergyType::Colorless, TEXT("/Game/ThirdParty/GameIcons/round-star.round-star"), TEXT("B8B2A2"));
}

void UCardEnergyIconWidget::SetEnergyType(ECardEnergyType InEnergyType)
{
	EnergyType = InEnergyType;
	
	const UCardEnergyIconWidget* Defaults = GetClass()->GetDefaultObject<UCardEnergyIconWidget>();
	
	const FCardEnergyStyle* Style = Defaults->Styles.Find(EnergyType);
	if (!Style) return;
	
	TypeIcon->SetBrushFromTexture(Style->Icon);
	CircleImage->SetColorAndOpacity(Style->CircleColor);
}

void UCardEnergyIconWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetEnergyType(EnergyType);
}