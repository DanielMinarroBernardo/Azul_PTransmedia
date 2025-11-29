#include "AzulStoryManagerSubsystem.h"
#include "Engine/Engine.h"

UDataTable* UAzulStoryManagerSubsystem::GetDialogueForNPC(FName NPC_ID)
{
    // 1. Comprobar que el NPC existe en la configuración
    if (!DialogueMappings.Contains(NPC_ID))
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC '%s' no tiene asignado ningún diálogo"), *NPC_ID.ToString());
        return nullptr;
    }

    FNPCDialogueMap& NPCMap = DialogueMappings[NPC_ID];

    // 2. ¿Este NPC tiene diálogo para la escena actual?
    if (!NPCMap.SceneDialogue.Contains(CurrentStoryScene))
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC '%s' no tiene diálogo en la escena %d"),
            *NPC_ID.ToString(), CurrentStoryScene);

        return nullptr;
    }

    FDialogueSceneEntry& SceneEntry = NPCMap.SceneDialogue[CurrentStoryScene];

    // 3. ¿El NPC ya completó su diálogo en esta escena?
    bool bCompleted = NPCCompletedDialogue.Contains(NPC_ID) ?
        NPCCompletedDialogue[NPC_ID] : false;

    if (bCompleted && !SceneEntry.bRepeatUntilCompleted)
    {
        // No debe repetir → no habla
        return nullptr;
    }

    // 4. Buscar variante
    if (!CurrentSceneVariant.IsNone() &&
        SceneEntry.VariantDialogues.Contains(CurrentSceneVariant))
    {
        return SceneEntry.VariantDialogues[CurrentSceneVariant];
    }

    UE_LOG(LogTemp, Warning, TEXT("StoryScene = %d, Variant = %s"), CurrentStoryScene, *CurrentSceneVariant.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Existe NPC_ID en DialogueMappings? %s"), DialogueMappings.Contains(NPC_ID) ? TEXT("SI") : TEXT("NO"));


    // 5. Usar el diálogo por defecto
    return SceneEntry.DefaultDialogue;
}

void UAzulStoryManagerSubsystem::OnNPCDialogueFinished(FName NPC_ID)
{
    // Marcar completado
    NPCCompletedDialogue.FindOrAdd(NPC_ID) = true;

    UE_LOG(LogTemp, Warning, TEXT("NPC %s terminó su diálogo en escena %d"),
        *NPC_ID.ToString(), CurrentStoryScene);
}

void UAzulStoryManagerSubsystem::AdvanceScene(int32 NewScene, FName NewVariant)
{
    CurrentStoryScene = NewScene;
    CurrentSceneVariant = NewVariant;

    // Resetear el estado de "completado" de TODOS los NPC
    NPCCompletedDialogue.Empty();

    UE_LOG(LogTemp, Warning, TEXT("La historia avanzó a escena %d (Variante=%s)"),
        CurrentStoryScene, *CurrentSceneVariant.ToString());
}

void UAzulStoryManagerSubsystem::ClearMappings()
{
    DialogueMappings.Empty();
}

void UAzulStoryManagerSubsystem::AddNPCDialogueEntry(
    FName NPC_ID,
    int32 Scene,
    UDataTable* DefaultTable,
    TMap<FName, UDataTable*> VariantTables,
    bool bRepeatUntilCompleted
)
{
    FNPCDialogueMap& NPCMap = DialogueMappings.FindOrAdd(NPC_ID);

    FDialogueSceneEntry Entry;
    Entry.DefaultDialogue = DefaultTable;
    Entry.VariantDialogues = VariantTables;
    Entry.bRepeatUntilCompleted = bRepeatUntilCompleted;

    NPCMap.SceneDialogue.Add(Scene, Entry);
}
