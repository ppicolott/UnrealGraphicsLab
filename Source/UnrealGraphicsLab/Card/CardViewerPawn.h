#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CardViewerPawn.generated.h"

class ACardActor;
class APlayerController;
class UCameraComponent;

// Drag with the left mouse button to spin it, R to reset, Right/Left (or D/A) for the next/previous card, mouse wheel to zoom.
UCLASS()
class UNREALGRAPHICSLAB_API ACardViewerPawn : public APawn
{
	GENERATED_BODY()
	
public:
	ACardViewerPawn();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void NotifyControllerChanged() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Viewer")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, Category = "Viewer")
	TArray<TObjectPtr<ACardActor>> Cards;
	
	UPROPERTY(EditAnywhere, Category = "Viewer|Zoom", meta = (ClampMin = 0, Units = "Centimeters"))
	float ViewDistance = 450.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer|Zoom", meta = (ClampMin = 0, Units = "Centimeters"))
	float MinViewDistance = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer|Zoom", meta = (ClampMin = 0, Units = "Centimeters"))
	float MaxViewDistance = 900.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer|Zoom", meta = (ClampMin = 0, Units = "Centimeters"))
	float ZoomStep = 50.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer|Zoom", meta = (ClampMin = 0))
	float ZoomSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer", meta = (ClampMin = 0, Units = "Centimeters"))
	float SlideDistance = 160.0f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer", meta = (ClampMin = 0.01, Units = "Seconds"))
	float SlideTime = 0.35f;
	
	UPROPERTY(EditAnywhere, Category = "Viewer", meta = (ClampMin = 0))
	float DragSpeed = 0.4f;
	
private:
	ACardActor* GetCard(int32 Index) const;
	FVector GetShowcaseCenter() const;
	
	void LayOutCards();
	void ShowCard(int32 Index, int32 Direction);
	void UpdateSlide(float DeltaSeconds);
	void FinishSlide();
	void HandleKeys(APlayerController& PlayerController);
	void HandleDrag(APlayerController& PlayerController);
	void UpdateZoom(float DeltaSeconds);
	
	bool bLaidOut = false;
	
	float TargetViewDistance = 0.0f;
	float CurrentViewDistance = 0.0f;
	int32 CurrentIndex = 0;
	
	int32 OutgoingIndex = INDEX_NONE;
	float SlideAlpha = 1.0f;
	int32 SlideDirection = 1;
	
	bool bDragging = false;
	FVector2D LastMousePosition = FVector2D::ZeroVector;
};