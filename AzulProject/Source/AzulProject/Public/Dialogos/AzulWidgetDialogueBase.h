// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AzulWidgetDialogueBase.generated.h"

class AAzulNPCBase;
/**
 * 
 */
UCLASS()
class AZULPROJECT_API UAzulWidgetDialogueBase : public UUserWidget
{
	GENERATED_BODY()

public:

    /* Referencia al NPC que controla el diálogo */
    UPROPERTY(BlueprintReadWrite, Category = "Azul|Dialogue")
    AAzulNPCBase* NPC;

    /* Llamado desde el botón continuar */
    UFUNCTION(BlueprintCallable, Category = "Azul|Dialogue")
    void PressContinue();

    UFUNCTION(BlueprintCallable, Category = "Azul|Dialogue")
    void PressChoice(int32 ChoiceIndex);

	
};
