#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AzulWidgetDialogueBase.generated.h"

class AAzulNPCBase;
class UAzulDialogue;

UCLASS()
class AZULPROJECT_API UAzulWidgetDialogueBase : public UUserWidget
{
    GENERATED_BODY()

public:

    /* Referencia al NPC que controla el diálogo */
    UPROPERTY(BlueprintReadWrite, Category = "Azul|Dialogue")
    AAzulNPCBase* NPC;

    /* Referencia al objeto diálogo activo */
    UPROPERTY(BlueprintReadWrite, Category = "Azul|Dialogue")
    UAzulDialogue* Dialogue;

    /* Llamado desde el botón continuar */
    UFUNCTION(BlueprintCallable, Category = "Azul|Dialogue")
    void PressContinue();

    UFUNCTION(BlueprintCallable, Category = "Azul|Dialogue")
    void PressChoice(int32 ChoiceIndex);

protected:

    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent
    ) override;
};
