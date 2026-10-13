#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardTextLibrary.generated.h"

class UUserWidget;
class UTextureRenderTarget2D;

UCLASS()
class UNREALGRAPHICSLAB_API UCardTextLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Card")
    static void DrawWidgetToRenderTarget(UUserWidget* Widget, UTextureRenderTarget2D* RenderTarget, float Scale = 2.0f);
};