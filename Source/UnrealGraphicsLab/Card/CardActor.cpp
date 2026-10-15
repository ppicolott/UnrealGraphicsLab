#include "CardActor.h"

#include "CardData.h"
#include "CardTextLibrary.h"
#include "CardTextWidget.h"

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ACardActor::SetCardData(UCardData* InCardData)
{
	CardData = InCardData;
	RedrawText();
}

void ACardActor::RedrawText()
{
	if (!CardData || !TextWidgetClass || !TextRenderTarget)
	{
		return;
	}

	UCardTextWidget* TextWidget = CreateWidget<UCardTextWidget>(GetWorld(), TextWidgetClass);
	if (!TextWidget)
	{
		return;
	}

	TextWidget->SetCardData(CardData);
	UCardTextLibrary::DrawWidgetToRenderTarget(TextWidget, TextRenderTarget, TextScale);
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();
	RedrawText();
}

#if WITH_EDITOR
void ACardActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ACardActor, CardData))
	{
		RedrawText();
	}
}
#endif