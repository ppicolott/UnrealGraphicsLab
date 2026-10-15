#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.generated.h"

class UCardData;
class UCardTextWidget;
class UTextureRenderTarget2D;

UCLASS(Abstract)
class UNREALGRAPHICSLAB_API ACardActor : public AActor
{
	GENERATED_BODY()

public:
	ACardActor();
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardData(UCardData* InCardData);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Card")
	void RedrawText();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UCardData> CardData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Card|Text")
	TSubclassOf<UCardTextWidget> TextWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Card|Text")
	TObjectPtr<UTextureRenderTarget2D> TextRenderTarget;
	
	UPROPERTY(EditAnywhere, Category = "Card|Text", meta = (ClampMin = 0.1))
	float TextScale = 2.0f;
};