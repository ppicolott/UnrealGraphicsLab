#include "CardActor.h"

#include "CardData.h"
#include "CardTextLibrary.h"
#include "CardTextWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Containers/Ticker.h"

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	CardMesh->SetupAttachment(RootComponent);
}

void ACardActor::SetCardData(UCardData* InCardData)
{
	CardData = InCardData;
	RefreshCard();
}

void ACardActor::RefreshCard()
{
	UpdateMaterial();
	RedrawText();
	
	// Textures and shaders may still be compiling when the level opens, so draw again shortly after
	FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateWeakLambda(this, [this](float)
			{
					RedrawText();
					return false;
			}),
			0.5f);
}

void ACardActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshCard();
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();
	RefreshCard();
}

void ACardActor::UpdateMaterial()
{
	if (!CardData || !CardData->CardMaterial)
	{
		return;
	}
	
	if (!TextTarget)
	{
		TextTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
				this, TextResolution.X, TextResolution.Y, RTF_RGBA8_SRGB, FLinearColor::Transparent);
	}
	
	CardMaterial = UMaterialInstanceDynamic::Create(CardData->CardMaterial, this);
	CardMaterial->SetTextureParameterValue(TEXT("TextLayer"), TextTarget);
	CardMesh->SetMaterial(0, CardMaterial);
}

void ACardActor::RedrawText()
{
	if (!CardData || !TextWidgetClass || !TextTarget)
	{
		return;
	}

	UCardTextWidget* TextWidget = CreateWidget<UCardTextWidget>(GetWorld(), TextWidgetClass);
	if (!TextWidget)
	{
		return;
	}

	TextWidget->SetCardData(CardData);
	UCardTextLibrary::DrawWidgetToRenderTarget(TextWidget, TextTarget, TextScale);
}