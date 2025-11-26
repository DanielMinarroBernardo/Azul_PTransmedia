#include "AzulDialogue.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h" // Para AddOnScreenDebugMessage
#include "Misc/OutputDeviceDebug.h"

void UAzulDialogue::StartDialogue()
{
    UE_LOG(LogTemp, Warning, TEXT("START DIALOGUE C++ LLAMADO"));

    CurrentID = 1;
    PlayerScore = 0;

    ChoiceButtons.Empty();

    CurrentTable = DialogueTable;

    LoadCurrentRow();
    UE_LOG(LogTemp, Warning, TEXT("EMITIENDO OnDialogueUpdated (StartDialogue)"));
    OnDialogueUpdated.Broadcast();
}


bool UAzulDialogue::LoadCurrentRow()
{
    if (!CurrentTable)
    {
        OnDialogueFinished.Broadcast();
        return false;
    }

    FName RowName = FName(*FString::FromInt(CurrentID));
    CurrentRow = CurrentTable->FindRow<FDialogueRow>(RowName, TEXT(""));

    UE_LOG(LogTemp, Warning, TEXT("LoadCurrentRow ejecutado. CurrentID = %d"), CurrentID);

    if (CurrentRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("Fila encontrada: %s"), *CurrentRow->Text);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ERROR: Fila con ID %d NO encontrada en la DataTable %s"),
            CurrentID,
            *CurrentTable->GetName());
    }


    return true;
}





FString UAzulDialogue::GetCurrentText() const
{
    if (!CurrentRow)
    {
        return FString("ERROR: No hay fila actual");
    }

    FString Text = CurrentRow->Text;

    // Reemplazar {SonName}
    if (!PlayerName.IsEmpty())
    {
        Text = Text.Replace(TEXT("{SonName}"), *PlayerName, ESearchCase::CaseSensitive);
    }

    return Text;
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
// SIEMPRE reconstruir los botones
    ChoiceButtons.Empty();

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
    if (CurrentRow->ChoicesScore.IsValidIndex(ButtonIndex))
        PlayerScore += CurrentRow->ChoicesScore[ButtonIndex];

    if (CurrentRow->ChoicesNext.IsValidIndex(ButtonIndex))
        CurrentID = CurrentRow->ChoicesNext[ButtonIndex];

    if (!LoadCurrentRow())
    {
        OnDialogueFinished.Broadcast();
        return;
    }

    OnDialogueUpdated.Broadcast();
}




void UAzulDialogue::ContinueDialogue()
{
    if (!CurrentRow)
        return;

    // Si NextID es 0 o no existe → diálogo terminado
    if (CurrentRow->NextID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("DIALOGO TERMINADO EN SEGUIDA — NextID <= 0"));
        OnDialogueFinished.Broadcast();
        return;
    }

    CurrentID = CurrentRow->NextID;

    if (!LoadCurrentRow())
    {
        OnDialogueFinished.Broadcast();
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

    UE_LOG(LogTemp, Warning, TEXT("SETDIALOGUETEXT ejecutado: %s"), *GetCurrentText());

}

void UAzulDialogue::SetDialogueTable(UDataTable* InTable)
{
    DialogueTable = InTable;
    CurrentTable = InTable;
}


