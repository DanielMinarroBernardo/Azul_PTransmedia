#include "Dialogos/AzulWidgetDialogueBase.h"
#include "Actors/AzulNPCBase.h"
#include "Dialogos/AzulDialogue.h"

void UAzulWidgetDialogueBase::PressContinue()
{
    if (NPC)
    {
        NPC->OnContinueRequested();
    }
}

void UAzulWidgetDialogueBase::PressChoice(int32 ChoiceIndex)
{
    if (NPC)
    {
        NPC->OnChoiceSelected(ChoiceIndex);
    }
}

FReply UAzulWidgetDialogueBase::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent
)
{
    // Solo botón izquierdo / touch
    if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    }

    // Si no hay diálogo, no hacer nada
    if (!Dialogue || !Dialogue->CurrentRow)
    {
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    }

    // Si es decisión → NO avanzar con click general
    if (Dialogue->CurrentRow->IsDecision)
    {
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    }

    // No es decisión → avanzar diálogo
    PressContinue();

    return FReply::Handled();
}
