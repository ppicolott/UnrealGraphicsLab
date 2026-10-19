#include "CardViewerPawn.h"

#include "CardActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

ACardViewerPawn::ACardViewerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;
	Camera->SetFieldOfView(40.0f); // narrow lens for less perspective distortion on the card
}

void ACardViewerPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void ACardViewerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!bLaidOut)
	{
		LayOutCards();
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		HandleKeys(*PlayerController);
		HandleDrag(*PlayerController);
		HandleGlare(*PlayerController);
	}
	
	UpdateSlide(DeltaSeconds);
	UpdateZoom(DeltaSeconds);
}

void ACardViewerPawn::UpdateZoom(float DeltaSeconds)
{
	CurrentViewDistance = FMath::FInterpTo(CurrentViewDistance, TargetViewDistance, DeltaSeconds, ZoomSpeed);
	
	if (SlideAlpha >= 1.0f)
	{
		if (ACardActor* Card = GetCard(CurrentIndex))
		{
			Card->PlaceFacing(GetShowcaseCenter(), -Camera->GetForwardVector());
		}
	}
}

ACardActor* ACardViewerPawn::GetCard(int32 Index) const
{
	return Cards.IsValidIndex(Index) ? Cards[Index].Get() : nullptr;
}

FVector ACardViewerPawn::GetShowcaseCenter() const
{
	return Camera->GetComponentLocation() + Camera->GetForwardVector() * CurrentViewDistance;
}

void ACardViewerPawn::LayOutCards()
{
	TargetViewDistance = FMath::Clamp(ViewDistance, MinViewDistance, MaxViewDistance);
	CurrentViewDistance = TargetViewDistance;
	
	for (int32 Index = 0; Index < Cards.Num(); ++Index)
	{
		if (ACardActor* Card = GetCard(Index))
		{
			Card->ResetUserRotation();
			Card->PlaceFacing(GetShowcaseCenter(), -Camera->GetForwardVector());
			Card->SetActorHiddenInGame(Index != CurrentIndex);
		}
	}
	
	bLaidOut = true;
}

void ACardViewerPawn::ShowCard(int32 Index, int32 Direction)
{
	if (Cards.Num() < 2)
	{
		return;
	}
	
	FinishSlide();
	
	OutgoingIndex = CurrentIndex;
	CurrentIndex = (Index % Cards.Num() + Cards.Num()) % Cards.Num(); // wrap around both ends
	SlideDirection = Direction;
	SlideAlpha = 0.0f;
	bDragging = false;
	
	if (ACardActor* Incoming = GetCard(CurrentIndex))
	{
		Incoming->ResetUserRotation();
		Incoming->SetActorHiddenInGame(false);
	}
	
	UpdateSlide(0.0f);
}

void ACardViewerPawn::UpdateSlide(float DeltaSeconds)
{
	if (SlideAlpha >= 1.0f)
	{
		return;
	}
	
	SlideAlpha = FMath::Min(1.0f, SlideAlpha + DeltaSeconds / SlideTime);
	const float Eased = FMath::InterpEaseInOut(0.0f, 1.0f, SlideAlpha, 2.0f);
	
	const FVector Center = GetShowcaseCenter();
	const FVector Slide = Camera->GetRightVector() * (SlideDirection * SlideDistance);
	const FVector ToViewer = -Camera->GetForwardVector();
	
	if (ACardActor* Incoming = GetCard(CurrentIndex))
	{
		Incoming->PlaceFacing(Center + Slide * (Eased - 1.0f), ToViewer);
	}
	if (ACardActor* Outgoing = GetCard(OutgoingIndex))
	{
		Outgoing->PlaceFacing(Center + Slide * Eased, ToViewer);
	}
	
	if (SlideAlpha >= 1.0f)
	{
		FinishSlide();
	}
}

void ACardViewerPawn::FinishSlide()
{
	const FVector ToViewer = -Camera->GetForwardVector();
	
	if (ACardActor* Incoming = GetCard(CurrentIndex))
	{
		Incoming->PlaceFacing(GetShowcaseCenter(), ToViewer);
	}
	
	if (ACardActor* Outgoing = GetCard(OutgoingIndex))
	{
		Outgoing->SetActorHiddenInGame(true);
		Outgoing->ResetUserRotation();
		Outgoing->PlaceFacing(GetShowcaseCenter(), ToViewer);
	}
	
	OutgoingIndex = INDEX_NONE;
	SlideAlpha = 1.0f;
}

void ACardViewerPawn::HandleKeys(APlayerController& PlayerController)
{
	auto Pressed = [&PlayerController](const FKey& Key) { return PlayerController.WasInputKeyJustPressed(Key); };
	
	if (Pressed(EKeys::Right) || Pressed(EKeys::D))
	{
		ShowCard(CurrentIndex + 1, 1);
	}
	else if (Pressed(EKeys::Left) || Pressed(EKeys::A))
	{
		ShowCard(CurrentIndex - 1, -1);
	}
	
	if (Pressed(EKeys::MouseScrollUp))
	{
		TargetViewDistance = FMath::Max(TargetViewDistance - ZoomStep, MinViewDistance);
	}
	else if (Pressed(EKeys::MouseScrollDown))
	{
		TargetViewDistance = FMath::Min(TargetViewDistance + ZoomStep, MaxViewDistance);
	}
	
	if (Pressed(EKeys::R))
	{
		if (ACardActor* Card = GetCard(CurrentIndex))
		{
			Card->ResetUserRotation();
		}
	}
}

void ACardViewerPawn::HandleDrag(APlayerController& PlayerController)
{
	float MouseX, MouseY;
	const bool bHasMouse = PlayerController.GetMousePosition(MouseX, MouseY);
	
	const FVector2D MousePosition(MouseX, MouseY);
	
	if (!bHasMouse || SlideAlpha < 1.0f || !PlayerController.IsInputKeyDown(EKeys::LeftMouseButton))
	{
		bDragging = false;
		return;
	}
	
	ACardActor* Card = GetCard(CurrentIndex);
	if (bDragging && Card)
	{
		const FVector2D Delta = MousePosition - LastMousePosition;
		Card->AddUserRotation(-Delta.X * DragSpeed, -Delta.Y * DragSpeed);
	}
	
	bDragging = true;
	LastMousePosition = MousePosition;
}

void ACardViewerPawn::HandleGlare(APlayerController& PlayerController)
{
	FVector RayOrigin, RayDirection;
	const bool bHasRay = SlideAlpha >= 1.0f && PlayerController.DeprojectMousePositionToWorld(RayOrigin, RayDirection);
	
	for (int32 Index = 0; Index < Cards.Num(); ++Index)
	{
		if (ACardActor* Card = GetCard(Index))
		{
			FVector2D UV = FVector2D::ZeroVector;
			const bool bHovering = bHasRay && Index == CurrentIndex && Card->TraceCardUV(RayOrigin, RayDirection, UV);
			Card->SetGlare(UV, bHovering);
		}
	}
}