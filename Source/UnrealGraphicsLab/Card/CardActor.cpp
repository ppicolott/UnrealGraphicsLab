#include "CardActor.h"

#include "CardData.h"
#include "CardTextLibrary.h"
#include "CardTextWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Containers/Ticker.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/StaticMesh.h"

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	CardMesh->SetupAttachment(RootComponent);
	CardMesh->bAffectDynamicIndirectLighting = false;
	
	BackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackMesh"));
	BackMesh->SetupAttachment(CardMesh);
	BackMesh->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	BackMesh->bAffectDynamicIndirectLighting = false;
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

bool ACardActor::TraceCardUV(const FVector& RayOrigin, const FVector& RayDirection, FVector2D& OutUV) const
{
	const FTransform& MeshTransform = CardMesh->GetComponentTransform();
	const FVector Normal = MeshTransform.GetUnitAxis(EAxis::Z);
	
	const double Facing = FVector::DotProduct(RayDirection, Normal);
	if (FMath::IsNearlyZero(Facing))
	{
		return false; // ray parallel to the card
	}
	
	const double Distance = FVector::DotProduct(MeshTransform.GetLocation() - RayOrigin, Normal) / Facing;
	if (Distance < 0.0)
	{
		return false; // card behind the camera
	}
	
	const FVector Local = MeshTransform.InverseTransformPosition(RayOrigin + RayDirection * Distance);
	const FBox Bounds = CardMesh->GetStaticMesh()->GetBoundingBox();
	OutUV = (FVector2D(Local) - FVector2D(Bounds.Min)) / FVector2D(Bounds.GetSize());
	return OutUV.X >= 0.0 && OutUV.X <= 1.0 && OutUV.Y >= 0.0 && OutUV.Y <= 1.0;
}

void ACardActor::SetGlare(FVector2D InUV, bool bInHovering)
{
	bGlareHovering = bInHovering;
	if (bInHovering)
	{
		GlareUV = InUV; // keep the last spot while it fades out
	}
}

void ACardActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshCard();
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();
	RestRotation = CardMesh->GetRelativeRotation().Quaternion();
	RefreshCard();
}

void ACardActor::UpdateMaterial()
{
	BackMesh->SetStaticMesh(CardMesh->GetStaticMesh());
	if (BackMaterial)
	{
		BackMesh->SetMaterial(0, BackMaterial);
	}
	
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

void ACardActor::RedrawText() const
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}
	
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
	
	if (PreviewTarget)
	{
		UCardTextLibrary::DrawWidgetToRenderTarget(TextWidget, PreviewTarget, TextScale);
	}
}

void ACardActor::SetTiltInput(FVector2D InTilt)
{
	TiltInput = InTilt.ClampAxes(-1.0, 1.0);
}

void ACardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const FQuat Turn(FVector::UpVector, FMath::DegreesToRadians(TiltInput.X * MaxTiltAngle));
	const FQuat Nod(FVector::ForwardVector, FMath::DegreesToRadians(TiltInput.Y * MaxTiltAngle));
	
	const FVector LocalFront = RestRotation * FVector::UpVector;
	const FVector Horizontal = FVector::CrossProduct(LocalFront, FVector::UpVector).GetSafeNormal();
	const FQuat UserTurn(FVector::UpVector, FMath::DegreesToRadians(UserYaw));
	const FQuat UserTip(Horizontal, FMath::DegreesToRadians(UserPitch));
	
	const FQuat Target = UserTip * UserTurn * Turn * Nod * RestRotation;
	
	const bool bUserRotated = !FMath::IsNearlyZero(UserYaw) || !FMath::IsNearlyZero(UserPitch);
	const float Speed = bUserRotated ? UserRotationSpeed : TiltSpeed;
	
	const FQuat Current = CardMesh->GetRelativeRotation().Quaternion();
	CardMesh->SetRelativeRotation(FMath::QInterpTo(Current, Target, DeltaSeconds, Speed));
	
	GlareAmount = FMath::FInterpTo(GlareAmount, bGlareHovering ? 1.0f : 0.0f, DeltaSeconds, GlareFadeSpeed);
	if (CardMaterial)
	{
		CardMaterial->SetScalarParameterValue(TEXT("GlareAmount"), GlareAmount);
		CardMaterial->SetVectorParameterValue(TEXT("GlarePosition"), FLinearColor(GlareUV.X, GlareUV.Y, 0.0f));
	}
}

void ACardActor::AddUserRotation(float DeltaYaw, float DeltaPitch)
{
	UserYaw = FRotator::NormalizeAxis(UserYaw + DeltaYaw);
	UserPitch = FMath::Clamp(UserPitch + DeltaPitch, -MaxUserPitch, MaxUserPitch);
}

void ACardActor::ResetUserRotation()
{
	UserYaw = 0.0f;
	UserPitch = 0.0f;
}

void ACardActor::PlaceFacing(const FVector& Center, const FVector& ToViewer)
{
	const FVector LocalFront = RestRotation * FVector::UpVector;
	
	const float LocalYaw = FMath::RadiansToDegrees(FMath::Atan2(LocalFront.Y, LocalFront.X));
	const float ViewerYaw = FMath::RadiansToDegrees(FMath::Atan2(ToViewer.Y, ToViewer.X));
	SetActorRotation(FRotator(0.0f, ViewerYaw - LocalYaw, 0.0f));
	
	SetActorLocation(Center - (CardMesh->GetComponentLocation() - GetActorLocation()));
}