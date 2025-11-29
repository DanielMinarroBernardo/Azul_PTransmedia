// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AzulInteractuableBase.h"
#include "AzulDialogue.h"
#include "AzulNPCBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AZULPROJECT_API AAzulNPCBase : public AAzulInteractuableBase
{
    GENERATED_BODY()
public:

    AAzulNPCBase();

protected:

    virtual void BeginPlay() override;

public:

    /** Esta clase ES un NPC --> puede tener diálogo */

    /* Motor interno del diálogo */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
    UAzulDialogue* DialogueSystem;

    /* Nombre del NPC para sustituir variables en el texto si quieres */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString NPCName;

    /* IMPORTANTE:
       Este valor identifica a este NPC en el StoryManager */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName NPC_ID;

    /* Widget blueprint que contiene la UI del diálogo */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    /* Instancia actual del widget de diálogo */
    UPROPERTY()
    UUserWidget* DialogueWidgetInstance;

    /**
     * FUNCIÓN CRÍTICA:
     * Se ejecuta cuando el jugador pulsa Interactuar (tecla E)
     */
    virtual void Interactua_Implementation() override;

    /* Llamado al terminar el diálogo */
    UFUNCTION()
    void OnNPCDialogueFinished();
};
