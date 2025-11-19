#include "AzulDialogue.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h" // Para AddOnScreenDebugMessage
#include "Misc/OutputDeviceDebug.h"

void UAzulDialogue::StartDialogue()
{
    CurrentTableIndex = 0;
    CurrentID = 1;
    PlayerScore = 0;

    if (DialogueTables.IsValidIndex(0))
    {
        CurrentTable = DialogueTables[0];  // Tabla fija siempre
    }

    LoadCurrentRow();
    OnDialogueUpdated.Broadcast();
}

bool UAzulDialogue::LoadCurrentRow()
{

    // 1. Validar índice de tabla
    if (!DialogueTables.IsValidIndex(CurrentTableIndex))
    {
        OnDialogueFinished.Broadcast();
        return false;
    }

    // 2. Establecer la tabla actual
    CurrentTable = DialogueTables[CurrentTableIndex];
    if (!CurrentTable)
    {
        OnDialogueFinished.Broadcast();
        return false;
    }

    // 3. Buscar la fila con el ID actual
    FName RowName = FName(*FString::FromInt(CurrentID));
    CurrentRow = CurrentTable->FindRow<FDialogueRow>(RowName, TEXT(""));

    UE_LOG(LogTemp, Warning, TEXT("Cargando fila ID=%d en tabla=%s"),
        CurrentID,
        *CurrentTable->GetName());


    // 4. Si no existe la fila → terminar diálogo (NO cambiar de DataTable)
    if (!CurrentRow)
    {
        UE_LOG(LogTemp, Error, TEXT("NO EXISTE la fila con ID=%d en la tabla %s"),
            CurrentID,
            *CurrentTable->GetName());
        OnDialogueFinished.Broadcast();
        return false;
    }


    //// 4. Si no existe la fila → pasar a la siguiente DataTable
    //if (!CurrentRow)
    //{
    //    // Pasar a la siguiente DataTable
    //    CurrentTableIndex++;

    //    // ¿Existen más DataTables?
    //    if (!DialogueTables.IsValidIndex(CurrentTableIndex))
    //    {
    //        OnDialogueFinished.Broadcast();
    //        return false;
    //    }

    //    // Reiniciar ID al empezar la nueva tabla
    //    CurrentID = 1;

    //    // Cargar la nueva tabla
    //    CurrentTable = DialogueTables[CurrentTableIndex];
    //    RowName = FName(*FString::FromInt(CurrentID));
    //    CurrentRow = CurrentTable->FindRow<FDialogueRow>(RowName, TEXT(""));

    //    return CurrentRow != nullptr;
    //}

    // 5. Fila cargada correctamente
    return true;
}




FString UAzulDialogue::GetCurrentText() const
{
    if (!CurrentRow)
    {
        OnDialogueFinished.Broadcast();
        return FString("ERROR: No hay fila actual");
    }

    return CurrentRow ? CurrentRow->Text : FString("ERROR: No hay fila actual");
}

void UAzulDialogue::UpdateWidget(UHorizontalBox* ChoicesContainer)
{
    if (!ChoicesContainer || !CurrentRow)
        return;

    if (ContinueButton)
    {
        ContinueButton->SetIsEnabled(!CurrentRow->IsDecision);
    }


    // 1. Primera vez: rellenar el array automáticamente desde el HorizontalBox
    if (ChoiceButtons.Num() == 0)
    {
        const int32 ChildCount = ChoicesContainer->GetChildrenCount();

        for (int32 i = 0; i < ChildCount; i++)
        {
            UWidget* Child = ChoicesContainer->GetChildAt(i);
            UButton* Btn = Cast<UButton>(Child);

            if (Btn)
            {
                ChoiceButtons.Add(Btn);
            }
        }
    }

    // Si no hay botones → salir
    if (ChoiceButtons.Num() == 0)
        return;

    // -------------------------------------------------------
    // 2. Si NO es decisión → ocultar todos los botones
    // -------------------------------------------------------
    if (!CurrentRow->IsDecision)
    {
        for (UButton* Btn : ChoiceButtons)
        {
            if (Btn)
                Btn->SetVisibility(ESlateVisibility::Collapsed);
        }

        return;
    }

    // -------------------------------------------------------
    // 3. Mostrar solo los necesarios y poner texto
    // -------------------------------------------------------

    int32 NumChoices = CurrentRow->ChoicesText.Num();

    for (int32 i = 0; i < ChoiceButtons.Num(); i++)
    {
        UButton* Btn = ChoiceButtons[i];
        if (!Btn) continue;

        if (i < NumChoices)
        {
            Btn->SetVisibility(ESlateVisibility::Visible);

            if (UTextBlock* Label = Cast<UTextBlock>(Btn->GetChildAt(0)))
            {
                Label->SetText(FText::FromString(CurrentRow->ChoicesText[i]));
            }
        }
        else
        {
            Btn->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}


void UAzulDialogue::HandleContinueClicked()
{
    ContinueDialogue();
    OnDialogueUpdated.Broadcast();
}

void UAzulDialogue::OnChoiceClicked(int32 ButtonIndex)
{
    // Actualizar puntuación si corresponde
    if (CurrentRow->ChoicesScore.IsValidIndex(ButtonIndex))
        PlayerScore += CurrentRow->ChoicesScore[ButtonIndex];

    // Cambiar ID
    if (CurrentRow->ChoicesNext.IsValidIndex(ButtonIndex))
        CurrentID = CurrentRow->ChoicesNext[ButtonIndex];

    LoadCurrentRow();
    OnDialogueUpdated.Broadcast();
}



void UAzulDialogue::ContinueDialogue()
{
    if (!CurrentRow) return;

    CurrentID = CurrentRow->NextID;

    if (!LoadCurrentRow())
    {
        return;
    }

    OnDialogueUpdated.Broadcast();
}


void UAzulDialogue::SetDialogueText(UTextBlock* Text)
{
    if (!Text)
    {
        return;
    }

    const FString CurrentText = GetCurrentText();
    Text->SetText(FText::FromString(CurrentText));
}

void UAzulDialogue::SetDialogueTables(const TArray<UDataTable*>& InTables)
{
    DialogueTables = InTables;

    // Reset
    CurrentTableIndex = 0;
    CurrentID = 1;
    CurrentRow = nullptr;

    if (DialogueTables.Num() > 0)
    {
        CurrentTable = DialogueTables[0];
    }
    else
    {
        CurrentTable = nullptr;
    }
}

