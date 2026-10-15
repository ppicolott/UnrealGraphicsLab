#include "CardTextLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Slate/WidgetRenderer.h"

void UCardTextLibrary::DrawWidgetToRenderTarget(UUserWidget* Widget, UTextureRenderTarget2D* RenderTarget, float Scale)
{
    if (!Widget || !RenderTarget || Scale <= 0.0f)
    {
        return;
    }
    
    FWidgetRenderer Renderer(/*bUseGammaCorrection*/true);
    const FVector2D DrawSize(RenderTarget->SizeX, RenderTarget->SizeY);
    Renderer.DrawWidget(RenderTarget, Widget->TakeWidget(), Scale, DrawSize, /*DeltaTime*/0.0f);
}