#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.generated.h"

class UCardData;
class UCardTextWidget;
class UTextureRenderTarget2D;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;

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
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetTiltInput(FVector2D InTilt);
	
	void AddUserRotation(float DeltaYaw, float DeltaPitch);
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void ResetUserRotation();
	
	void PlaceFacing(const FVector& Center, const FVector& ToViewer);
	
	bool TraceCardUV(const FVector& RayOrigin, const FVector& RayDirection, FVector2D& OutUV) const;
	void SetGlare(FVector2D InUV, bool bInHovering);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Card|Motion", meta = (ClampMin = 0, ClampMax = 45, Units = "Degrees"))
	float MaxTiltAngle = 15.0f;
	
	UPROPERTY(EditAnywhere, Category = "Card|Motion", meta = (ClampMin = 0))
	float TiltSpeed = 8.0f;
	
	UPROPERTY(EditAnywhere, Category = "Card|Motion", meta = (ClampMin = 0, ClampMax = 89, Units = "Degrees"))
	float MaxUserPitch = 30.0f;
	
	UPROPERTY(EditAnywhere, Category = "Card|Motion", meta = (ClampMin = 0))
	float UserRotationSpeed = 20.0f;
	
	UPROPERTY(EditAnywhere, Transient, Category = "Card|Motion")
	FVector2D TiltInput = FVector2D::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UStaticMeshComponent> CardMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UStaticMeshComponent> BackMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UMaterialInterface> BackMaterial;
	
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
	
	UPROPERTY(EditAnywhere, Category = "Card|Glare", meta = (ClampMin = 0))
	float GlareFadeSpeed = 6.0f;
	
	UPROPERTY(EditAnywhere, Category = "Card|Glare", meta = (ClampMin = 0))
	float GlareFollowSpeed = 36.0f;
	
private:
	void UpdateMaterial();
	void RedrawText() const;
	
	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> TextTarget;
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> CardMaterial;
	
	FQuat RestRotation = FQuat::Identity;
	
	float UserYaw = 0.0f;
	float UserPitch = 0.0f;
	
	FVector2D GlareUV = FVector2D(0.5, 0.5);
	bool bGlareHovering = false;
	float GlareAmount = 0.0f;
	FVector2D GlareTargetUV = FVector2D(0.5, 0.5);
};