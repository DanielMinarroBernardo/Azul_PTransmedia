#include "AzulDialogue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"

UAzulDialogue::UAzulDialogue()
{
    CurrentSceneIndex = 0;
    CurrentDialogueID = 1;
    PlayerScore = 0;
}

bool UAzulDialogue::LoadDialogueJSON()
{
    FString FilePath = FPaths::ProjectContentDir() / TEXT("Data/dialogues.json");
    FString JsonStr;

    if (!FFileHelper::LoadFileToString(JsonStr, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo leer el archivo JSON en %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> Root;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);

    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Error al parsear el JSON."));
        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>* JsonScenes;
    if (!Root->TryGetArrayField(TEXT("Scenes"), JsonScenes))
    {
        UE_LOG(LogTemp, Error, TEXT("No se encontró el campo 'Scenes' en el JSON."));
        return false;
    }

    Scenes.Empty();

    for (auto SceneValue : *JsonScenes)
    {
        TSharedPtr<FJsonObject> SceneObj = SceneValue->AsObject();
        if (!SceneObj.IsValid()) continue;

        FDialogueScene Scene;
        Scene.SceneID = SceneObj->GetIntegerField(TEXT("SceneID"));
        Scene.SceneName = SceneObj->GetStringField(TEXT("SceneName"));

        const TArray<TSharedPtr<FJsonValue>>* JsonDialogues;
        if (SceneObj->TryGetArrayField(TEXT("Dialogues"), JsonDialogues))
        {
            for (auto DValue : *JsonDialogues)
            {
                TSharedPtr<FJsonObject> DObj = DValue->AsObject();
                if (!DObj.IsValid()) continue;

                FDialogueNode Node;
                Node.ID = DObj->GetIntegerField(TEXT("ID"));
                Node.Type = DObj->GetStringField(TEXT("Type"));
                Node.Speaker = DObj->GetStringField(TEXT("Speaker"));
                Node.Text = DObj->GetStringField(TEXT("Text"));
                Node.NextID = DObj->HasTypedField<EJson::Number>(TEXT("NextID")) ? DObj->GetIntegerField(TEXT("NextID")) : -1;

                if (Node.Type == TEXT("decision"))
                {
                    const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
                    if (DObj->TryGetArrayField(TEXT("Choices"), ChoicesArray))
                    {
                        for (auto CValue : *ChoicesArray)
                        {
                            TSharedPtr<FJsonObject> CObj = CValue->AsObject();
                            if (!CObj.IsValid()) continue;

                            FDialogueChoice Choice;
                            Choice.Text = CObj->GetStringField(TEXT("Text"));
                            Choice.NextID = CObj->GetIntegerField(TEXT("NextID"));
                            Choice.Score = CObj->HasField(TEXT("Score")) ? CObj->GetIntegerField(TEXT("Score")) : 0;
                            Node.Choices.Add(Choice);
                        }
                    }
                }

                Scene.Dialogues.Add(Node);
            }
        }

        Scenes.Add(Scene);
    }

    UE_LOG(LogTemp, Log, TEXT("Se cargaron %d escenas desde el JSON."), Scenes.Num());
    return true;
}

void UAzulDialogue::StartScene(int32 SceneIndex)
{
    if (!Scenes.IsValidIndex(SceneIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Índice de escena inválido: %d"), SceneIndex);
        return;
    }

    CurrentSceneIndex = SceneIndex;
    CurrentDialogueID = 1;
    PlayerScore = 0;
}

void UAzulDialogue::AdvanceDialogue(int32 NextID)
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == NextID; });

    if (!Node)
    {
        UE_LOG(LogTemp, Warning, TEXT("No se encontró el diálogo con ID %d"), NextID);
        return;
    }

    CurrentDialogueID = NextID;
}

void UAzulDialogue::ChooseOption(int32 ChoiceIndex)
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; });

    if (!Node || Node->Choices.Num() <= ChoiceIndex) return;

    const FDialogueChoice& Choice = Node->Choices[ChoiceIndex];
    PlayerScore += Choice.Score;

    AdvanceDialogue(Choice.NextID);
}

FString UAzulDialogue::GetCurrentDialogueText() const
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex))
        return TEXT("");

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; });

    return Node ? Node->Text : TEXT("");
}

FDialogueScene UAzulDialogue::GetSceneByIndex(int32 SceneIndex) const
{
    if (Scenes.IsValidIndex(SceneIndex))
        return Scenes[SceneIndex];
    return FDialogueScene();
}

void UAzulDialogue::UpdateDecisionButtons(UHorizontalBox* ChoicesContainer)
{
    if (!ChoicesContainer || !Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; });

    if (!Node) return;

    const TArray<UWidget*> Children = ChoicesContainer->GetAllChildren();

    // Si el diálogo terminó, ocultar todos los botones
    if (Node->NextID == -1 && Node->Type != TEXT("decision"))
    {
        for (UWidget* Child : Children)
        {
            if (UButton* Button = Cast<UButton>(Child))
                Button->SetVisibility(ESlateVisibility::Collapsed);
        }
        return;
    }

    // --- Si es un nodo de decisión ---
    if (Node->Type == TEXT("decision"))
    {
        const int32 NumChoices = Node->Choices.Num();

        for (int32 i = 0; i < Children.Num(); ++i)
        {
            if (UButton* Button = Cast<UButton>(Children[i]))
            {
                // Buscar el TextBlock hijo
                UTextBlock* TextBlock = nullptr;
                const TArray<UWidget*> ButtonChildren = Button->GetAllChildren();
                for (UWidget* SubChild : ButtonChildren)
                {
                    if (UTextBlock* TB = Cast<UTextBlock>(SubChild))
                    {
                        TextBlock = TB;
                        break;
                    }
                }

                if (i < NumChoices)
                {
                    Button->SetVisibility(ESlateVisibility::Visible);
                    if (TextBlock)
                        TextBlock->SetText(FText::FromString(Node->Choices[i].Text));
                }
                else
                {
                    Button->SetVisibility(ESlateVisibility::Collapsed);
                }
            }
        }
    }
    else
    {
        // --- Si es un nodo normal (solo texto con "Continuar") ---
        bool bShown = false;
        for (UWidget* Child : Children)
        {
            if (UButton* Button = Cast<UButton>(Child))
            {
                if (!bShown)
                {
                    // Mostrar solo el primer botón como “Continuar”
                    Button->SetVisibility(ESlateVisibility::Visible);

                    // Cambiar texto a "Continuar"
                    const TArray<UWidget*> ButtonChildren = Button->GetAllChildren();
                    for (UWidget* SubChild : ButtonChildren)
                    {
                        if (UTextBlock* TB = Cast<UTextBlock>(SubChild))
                        {
                            TB->SetText(FText::FromString(TEXT("Continuar")));
                            break;
                        }
                    }
                    bShown = true;
                }
                else
                {
                    Button->SetVisibility(ESlateVisibility::Collapsed);
                }
            }
        }
    }
}

void UAzulDialogue::HandleChoiceSelection(int32 ChoiceIndex, UHorizontalBox* ChoicesContainer)
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; });

    if (!Node) return;

    if (Node->Type == TEXT("decision"))
    {
        ChooseOption(ChoiceIndex);
    }
    else
    {
        // Si es diálogo normal, usar NextID directamente
        if (Node->NextID != -1)
            AdvanceDialogue(Node->NextID);
        else
        {
            // Fin del diálogo
            UE_LOG(LogTemp, Log, TEXT("Fin del diálogo."));
        }
    }

    // 🔹 Actualizar los botones y texto tras avanzar
    UpdateDecisionButtons(ChoicesContainer);
}
