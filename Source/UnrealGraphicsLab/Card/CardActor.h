#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.generated.h"

class UCardData;
class UCardTextWidget;
class UTextureRenderTarget2D;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS(Abstract)
class UNREALGRAPHICSLAB_API ACardActor : public AActor
{
	GENERATED_BODY()
	
public:
	ACardActor();
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardData(UCardData* InCardData);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Card")
	void RefreshCard();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UStaticMeshComponent> CardMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UCardData> CardData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Card|Text")
	TSubclassOf<UCardTextWidget> TextWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Card|Text", meta = (ClampMin = 0.1))
	float TextScale = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Card|Text")
	FIntPoint TextResolution = FIntPoint(1260, 1760);
	
	// Optional extra target, e.g. RT_CardText, so the M_Card editor preview shows this card's text
	UPROPERTY(EditAnywhere, Category = "Card|Text")
	TObjectPtr<UTextureRenderTarget2D> PreviewTarget;
	
private:
	void UpdateMaterial();
	void RedrawText() const;
	
	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> TextTarget;
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> CardMaterial;
};