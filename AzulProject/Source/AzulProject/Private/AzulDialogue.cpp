#include "AzulDialogue.h"

UAzulDialogue::UAzulDialogue()
{
    CurrentID = 1;
    PlayerScore = 0;
    CurrentSceneIndex = 0;
    bSceneCompletedCorrectly = false;
}

void UAzulDialogue::InitScenes(const TArray<UDataTable*>& Scenes)
{
    SceneTables = Scenes;

    if (SceneTables.IsValidIndex(0))
    {
        DialogueTable = SceneTables[0];
    }

    CurrentSceneIndex = 0;
    CurrentID = 1;
    PlayerScore = 0;
}

void UAzulDialogue::StartScene()
{
    CurrentID = 1;
    PlayerScore = 0;
}

FDialogueRow* UAzulDialogue::GetCurrentRow()
{
    if (!DialogueTable) return nullptr;

    FName RowName = FName(*FString::FromInt(CurrentID));
    return DialogueTable->FindRow<FDialogueRow>(RowName, TEXT(""));
}

FString UAzulDialogue::GetDialogueText()
{
    FDialogueRow* Row = GetCurrentRow();
    return Row ? Row->Text : TEXT("");
}

bool UAzulDialogue::IsDecision()
{
    FDialogueRow* Row = GetCurrentRow();
    return Row ? Row->IsDecision : false;
}

TArray<FString> UAzulDialogue::GetChoiceTexts()
{
    FDialogueRow* Row = GetCurrentRow();
    return Row ? Row->ChoicesText : TArray<FString>();
}

void UAzulDialogue::SelectChoice(int32 Index)
{
    FDialogueRow* Row = GetCurrentRow();
    if (!Row) return;

    if (Row->ChoicesNext.IsValidIndex(Index))
    {
        PlayerScore += Row->ChoicesScore[Index];
        CurrentID = Row->ChoicesNext[Index];
    }
}

bool UAzulDialogue::EvaluateScene()
{
    return PlayerScore >= 0;
}

void UAzulDialogue::AdvanceSceneLogic()
{
    bSceneCompletedCorrectly = EvaluateScene();

    if (bSceneCompletedCorrectly)
    {
        // avanzar a la siguiente escena
        CurrentSceneIndex++;
    }
    else
    {
        // repetir escena: CurrentSceneIndex no cambia
    }

    // Seguridad
    CurrentSceneIndex = FMath::Clamp(CurrentSceneIndex, 0, SceneTables.Num() - 1);

    // cambiar a la nueva DataTable
    DialogueTable = SceneTables[CurrentSceneIndex];

    // reiniciar diálogo interno para la próxima vez que se abra
    CurrentID = 1;
    PlayerScore = 0;
}

void UAzulDialogue::Continue()
{
    FDialogueRow* Row = GetCurrentRow();
    if (!Row) return;

    CurrentID = Row->NextID;

    if (CurrentID == -1)
    {
        // se terminó la escena → decidir si avanzar o repetir
        AdvanceSceneLogic();

        // avisar al widget para que se cierre
        OnDialogueFinished.Broadcast();

        return;
    }
}

void UAzulDialogue::GetButtonData(int32 ButtonIndex, bool& bIsVisible, FString& OutText)
{
    bIsVisible = false;
    OutText = "";

    FDialogueRow* Row = GetCurrentRow();
    if (!Row) return;

    if (!Row->IsDecision)
    {
        // NO ES DECISIÓN → SOLO EL BOTÓN 0 ES VISIBLE
        if (ButtonIndex == 0)
        {
            bIsVisible = true;
            OutText = "Continuar";
        }
        return;
    }

    // ES DECISIÓN → VARIOS BOTONES
    if (Row->ChoicesText.IsValidIndex(ButtonIndex))
    {
        bIsVisible = true;
        OutText = Row->ChoicesText[ButtonIndex];
    }
}

void UAzulDialogue::OnButtonPressed(int32 ButtonIndex)
{
    FDialogueRow* Row = GetCurrentRow();
    if (!Row) return;

    if (!Row->IsDecision)
    {
        // Botón 0 es continuar
        Continue();
        return;
    }

    // Es decisión
    if (Row->ChoicesNext.IsValidIndex(ButtonIndex))
    {
        PlayerScore += Row->ChoicesScore[ButtonIndex];
        CurrentID = Row->ChoicesNext[ButtonIndex];

        // Después de seleccionar, actualizar flujo
        if (CurrentID == -1)
        {
            AdvanceSceneLogic();
            OnDialogueFinished.Broadcast();
        }
    }
}

void UAzulDialogue::IsSceneFinished(bool& bIsFinished)
{
    bIsFinished = (CurrentID == -1);
}

void UAzulDialogue::SetCurrentButtons(UHorizontalBox* ChoiceContainer)
{
    if (!ChoiceContainer) return;

    TArray<UWidget*> Children = ChoiceContainer->GetAllChildren();

    // Convertir widgets en botones y textblocks
    TArray<UButton*> Buttons;
    TArray<UTextBlock*> ButtonTexts;

    for (UWidget* Child : Children)
    {
        // Intentar obtener un botón
        UButton* Btn = Cast<UButton>(Child);
        if (Btn)
        {
            Buttons.Add(Btn);

            // Leer el textblock que contiene
            UTextBlock* Txt = Btn->GetChildAt(0) ? Cast<UTextBlock>(Btn->GetChildAt(0)) : nullptr;
            ButtonTexts.Add(Txt);
        }
    }

    // *** CASO 1: NO ES DECISIÓN ***
    if (!IsDecision())
    {
        for (int32 i = 0; i < Buttons.Num(); i++)
        {
            if (i == 0)
            {
                Buttons[i]->SetVisibility(ESlateVisibility::Visible);
                if (ButtonTexts[i])
                    ButtonTexts[i]->SetText(FText::FromString("Continuar"));
            }
            else
            {
                Buttons[i]->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
        return;
    }

    // *** CASO 2: ES DECISIÓN ***
    TArray<FString> Choices = GetChoiceTexts();

    int32 NumChoices = Choices.Num();
    int32 NumButtons = Buttons.Num();

    for (int32 i = 0; i < NumButtons; i++)
    {
        if (i < NumChoices)
        {
            Buttons[i]->SetVisibility(ESlateVisibility::Visible);
            if (ButtonTexts[i])
                ButtonTexts[i]->SetText(FText::FromString(Choices[i]));
        }
        else
        {
            Buttons[i]->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}
