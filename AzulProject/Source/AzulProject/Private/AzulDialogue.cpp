#include "AzulDialogue.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h" // Para AddOnScreenDebugMessage
#include "Misc/OutputDeviceDebug.h"

void UAzulDialogue::StartDialogue()
{
    CurrentTableIndex = 0;
    CurrentID = 1;
    PlayerScore = 0;

    UE_LOG(LogTemp, Log, TEXT("UAzulDialogue::StartDialogue - Tables: %d"), DialogueTables.Num());
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("StartDialogue - Tables: %d"), DialogueTables.Num()));
    }

    LoadCurrentRow();
    OnDialogueUpdated.Broadcast();
}

bool UAzulDialogue::LoadCurrentRow()
{
    UE_LOG(LogTemp, Log, TEXT("LoadCurrentRow - TableIndex=%d CurrentID=%d"), CurrentTableIndex, CurrentID);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            FString::Printf(TEXT("LoadCurrentRow - TableIndex=%d  CurrentID=%d"), CurrentTableIndex, CurrentID));
    }

    // 1. Validar índice de tabla
    if (!DialogueTables.IsValidIndex(CurrentTableIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadCurrentRow - Invalid table index %d"), CurrentTableIndex);
        OnDialogueFinished.Broadcast();
        return false;
    }

    // 2. Establecer la tabla actual
    CurrentTable = DialogueTables[CurrentTableIndex];
    if (!CurrentTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadCurrentRow - CurrentTable es NULL en index %d"), CurrentTableIndex);
        OnDialogueFinished.Broadcast();
        return false;
    }

    // 3. Buscar la fila con el ID actual
    FName RowName = FName(*FString::FromInt(CurrentID));
    CurrentRow = CurrentTable->FindRow<FDialogueRow>(RowName, TEXT(""));

    // 4. Si no existe, pasar a la siguiente tabla
    if (!CurrentRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadCurrentRow - Row %d no existe en tabla %s"),
            CurrentID, *CurrentTable->GetName());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
                FString::Printf(TEXT("Row %d no existe en tabla %s"), CurrentID, *CurrentTable->GetName()));
        }

        // Pasar a la siguiente DataTable
        CurrentTableIndex++;

        // No hay más tablas → diálogo terminado
        if (!DialogueTables.IsValidIndex(CurrentTableIndex))
        {
            UE_LOG(LogTemp, Log, TEXT("LoadCurrentRow - No hay más DataTables"));
            OnDialogueFinished.Broadcast();
            return false;
        }

        // Volver a empezar desde ID = 1
        CurrentID = 1;

        CurrentTable = DialogueTables[CurrentTableIndex];
        if (!CurrentTable)
        {
            UE_LOG(LogTemp, Error, TEXT("LoadCurrentRow - New table es NULL"));
            OnDialogueFinished.Broadcast();
            return false;
        }

        RowName = FName(*FString::FromInt(CurrentID));
        CurrentRow = CurrentTable->FindRow<FDialogueRow>(RowName, TEXT(""));

        if (!CurrentRow)
        {
            UE_LOG(LogTemp, Error, TEXT("LoadCurrentRow - La nueva tabla NO tiene fila ID=1"));
            OnDialogueFinished.Broadcast();
            return false;
        }
    }

    // 5. Todo OK → fila cargada correctamente
    UE_LOG(LogTemp, Log, TEXT("LoadCurrentRow - Loaded ID=%d : %s"),
        CurrentRow->ID, *CurrentRow->Text);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White,
            FString::Printf(TEXT("Loaded Row ID=%d : %s"), CurrentRow->ID, *CurrentRow->Text));
    }

    return true;
}


FString UAzulDialogue::GetCurrentText() const
{
    if (!CurrentRow)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("GetCurrentText - ERROR: No hay fila actual"));
        }
        UE_LOG(LogTemp, Warning, TEXT("GetCurrentText - No CurrentRow"));
        return FString("ERROR: No hay fila actual");
    }

    return CurrentRow ? CurrentRow->Text : FString("ERROR: No hay fila actual");
}

void UAzulDialogue::UpdateWidget(UHorizontalBox* ChoicesContainer)
{
    if (!ChoicesContainer || !CurrentRow)
        return;

    ChoicesContainer->ClearChildren();

    // Si NO es una decisión NO añadimos nada (tu botón de Continuar está en el Widget)
    if (!CurrentRow->IsDecision)
    {
        UE_LOG(LogTemp, Log, TEXT("UpdateWidget - Non-decision row, using BP Continue button"));
        return;
    }

    //  SI es decisión → creamos los botones
    UE_LOG(LogTemp, Log, TEXT("UpdateWidget - Decision row with %d choices"), CurrentRow->ChoicesText.Num());

    for (int32 i = 0; i < CurrentRow->ChoicesText.Num(); i++)
    {
        UButton* Button = NewObject<UButton>(ChoicesContainer);

        UTextBlock* Label = NewObject<UTextBlock>(Button);
        Label->SetText(FText::FromString(CurrentRow->ChoicesText[i]));
        Button->AddChild(Label);

        // Guardamos el índice
        PendingChoiceIndex = i;

        // Asignar evento dinámico
        Button->OnClicked.AddDynamic(this, &UAzulDialogue::HandleChoiceClicked);

        // Añadir al HorizontalBox con Slot personalizado
        UHorizontalBoxSlot* Slot = Cast<UHorizontalBoxSlot>(ChoicesContainer->AddChild(Button));
        if (Slot)
        {
            // Espaciado (tú puedes cambiarlo)
            Slot->SetPadding(FMargin(10.f, 0.f));

            // Que se estire horizontalmente
            Slot->SetHorizontalAlignment(HAlign_Fill);

            // Que ocupe el espacio proporcional
            Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        }
    }
}


void UAzulDialogue::HandleContinueClicked()
{
    UE_LOG(LogTemp, Log, TEXT("HandleContinueClicked - clicked"));
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Continue clicked"));
    }

    ContinueDialogue();
    OnDialogueUpdated.Broadcast();
}

void UAzulDialogue::HandleChoiceClicked()
{
    UE_LOG(LogTemp, Log, TEXT("HandleChoiceClicked - clicked PendingChoiceIndex=%d"), PendingChoiceIndex);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
            FString::Printf(TEXT("Choice clicked: index=%d"), PendingChoiceIndex));
    }

    OnChoiceClicked(PendingChoiceIndex);
    OnDialogueUpdated.Broadcast();
}

void UAzulDialogue::ContinueDialogue()
{
    if (!CurrentRow) return;

    UE_LOG(LogTemp, Log, TEXT("ContinueDialogue - CurrentID=%d NextID=%d"), CurrentID, CurrentRow->NextID);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            FString::Printf(TEXT("ContinueDialogue - Moving from %d to %d"), CurrentID, CurrentRow->NextID));
    }

    CurrentID = CurrentRow->NextID;

    if (!LoadCurrentRow())
    {
        UE_LOG(LogTemp, Warning, TEXT("ContinueDialogue - LoadCurrentRow returned false"));
        return;
    }

    OnDialogueUpdated.Broadcast();
}

void UAzulDialogue::OnChoiceClicked(int32 ButtonIndex)
{
    if (!CurrentRow) return;

    UE_LOG(LogTemp, Log, TEXT("OnChoiceClicked - ButtonIndex=%d"), ButtonIndex);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald,
            FString::Printf(TEXT("OnChoiceClicked - index=%d"), ButtonIndex));
    }

    if (!CurrentRow->ChoicesNext.IsValidIndex(ButtonIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnChoiceClicked - Invalid choice index %d"), ButtonIndex);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red,
                FString::Printf(TEXT("Invalid choice index %d"), ButtonIndex));
        }
        return;
    }

    if (CurrentRow->ChoicesScore.IsValidIndex(ButtonIndex))
    {
        int32 ScoreBefore = PlayerScore;
        PlayerScore += CurrentRow->ChoicesScore[ButtonIndex];
        UE_LOG(LogTemp, Log, TEXT("OnChoiceClicked - Score changed: %d -> %d (delta %d)"),
            ScoreBefore, PlayerScore, CurrentRow->ChoicesScore[ButtonIndex]);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue,
                FString::Printf(TEXT("Score: %d (+%d)"), PlayerScore, CurrentRow->ChoicesScore[ButtonIndex]));
        }
    }

    CurrentID = CurrentRow->ChoicesNext[ButtonIndex];

    UE_LOG(LogTemp, Log, TEXT("OnChoiceClicked - New CurrentID=%d"), CurrentID);
    if (!LoadCurrentRow())
    {
        UE_LOG(LogTemp, Warning, TEXT("OnChoiceClicked - LoadCurrentRow returned false after selecting choice"));
        return;
    }

    OnDialogueUpdated.Broadcast();
}

void UAzulDialogue::SetDialogueText(UTextBlock* Text)
{
    if (!Text)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetDialogueText - Null Text block"));
        return;
    }

    const FString CurrentText = GetCurrentText();
    UE_LOG(LogTemp, Log, TEXT("SetDialogueText - Setting text: %s"), *CurrentText);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
            FString::Printf(TEXT("Dialog text: %s"), *CurrentText));
    }

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

