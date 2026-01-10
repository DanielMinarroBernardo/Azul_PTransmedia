// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "AzulWidgetHUDPlayer.generated.h"

UENUM(BlueprintType)
enum class EInteractUIState : uint8
{
    Default             UMETA(DisplayName = "Default"),
    InRange             UMETA(DisplayName = "In Range"),
    InRangeAndLooking   UMETA(DisplayName = "In Range And Looking")
};


/**
 * 
 */
UCLASS()
class AZULPROJECT_API UAzulWidgetHUDPlayer : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // Imagen del widget (marcada IsVariable en UMG)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* InteractImage;

    /*UPROPERTY(meta = (BindWidget))
    UTextBlock* TextInteractuable;

    UPROPERTY(meta = (BindWidget))
    UBorder* BorderText;*/

    // Texturas configurables desde Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interact UI")
    UTexture2D* DefaultTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interact UI")
    UTexture2D* InRangeTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interact UI")
    UTexture2D* LookingTexture;

    UFUNCTION(BlueprintCallable)
    void SetUIState(EInteractUIState NewState);
};
