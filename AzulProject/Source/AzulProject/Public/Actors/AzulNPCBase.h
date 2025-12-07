#pragma once

#include "CoreMinimal.h"
#include "Actors/AzulInteractuableBase.h"
#include "Dialogos/AzulDialogue.h"
#include "Dialogos/AzulDialogueMappingDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
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

    /** Esta clase ES un NPC que puede tener diálogo */

    /* Motor interno del diálogo */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Dialogue")
    UAzulDialogue* DialogueSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Dialogue")
    FGameplayTag NPCDialogueBaseTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Dialogue")
    UAzulDialogueMappingDataAsset* DialogueMappingDataAsset;

    /* Widget blueprint que contiene la UI del diálogo */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Dialogue")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    /* Instancia actual del widget de diálogo */
    UPROPERTY()
    UUserWidget* DialogueWidgetInstance;


    virtual void Interactua_Implementation() override;

    UFUNCTION()
    void OnNPCDialogueUpdated();

    UFUNCTION()
    void OnNPCDialogueFinished();

    UFUNCTION()
    void OnContinueRequested();

    UFUNCTION()
    void OnChoiceSelected(int32 Index);


};
