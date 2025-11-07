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

                if (DObj->HasTypedField<EJson::Number>(TEXT("NextID")))
                    Node.NextID = DObj->GetIntegerField(TEXT("NextID"));
                else
                    Node.NextID = -1;

                // Si el tipo es "decision", leer sus Choices
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

TArray<FDialogueNode> UAzulDialogue::GetDialoguesByScene(int32 SceneIndex) const
{
    if (Scenes.IsValidIndex(SceneIndex))
        return Scenes[SceneIndex].Dialogues;
    return {};
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

    AdvanceDialogue(CurrentDialogueID);
}

void UAzulDialogue::AdvanceDialogue(int32 NextID)
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate([&](const FDialogueNode& N) { return N.ID == NextID; });

    if (!Node)
    {
        UE_LOG(LogTemp, Warning, TEXT("No se encontró el diálogo con ID %d"), NextID);
        return;
    }

    CurrentDialogueID = NextID;

    // 🔹 Dispara evento de actualización de diálogo
    OnDialogueUpdated.Broadcast(*Node);

    // 🔹 Si hay decisiones, dispara evento de creación de opciones
    if (Node->Type == TEXT("decision") && Node->Choices.Num() > 0)
    {
        OnChoicesCreated.Broadcast(Node->Choices);
    }
}



void UAzulDialogue::ChooseOption(int32 ChoiceIndex)
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex)) return;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate([&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; });

    if (!Node || Node->Choices.Num() <= ChoiceIndex)
    {
        UE_LOG(LogTemp, Warning, TEXT("Opción inválida: %d"), ChoiceIndex);
        return;
    }

    const FDialogueChoice& Choice = Node->Choices[ChoiceIndex];
    PlayerScore += Choice.Score;

    UE_LOG(LogTemp, Log, TEXT("Puntuación actual: %d"), PlayerScore);

    AdvanceDialogue(Choice.NextID);
}

int32 UAzulDialogue::GetCurrentChoicesCount() const
{
    if (!Scenes.IsValidIndex(CurrentSceneIndex))
        return 0;

    const FDialogueScene& Scene = Scenes[CurrentSceneIndex];
    const FDialogueNode* Node = Scene.Dialogues.FindByPredicate(
        [&](const FDialogueNode& N) { return N.ID == CurrentDialogueID; }
    );

    if (!Node)
        return 0;

    return Node->Choices.Num();
}

