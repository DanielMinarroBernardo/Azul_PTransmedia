#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "AzulStoryManagerSubsystem.generated.h"

/**
 * Estructura que define QUÉ diálogos tiene un NPC en cada escena.
 * Esto lo asignarás en el editor para cada NPC_ID.
 */
USTRUCT(BlueprintType)
struct FDialogueSceneEntry
{
    GENERATED_BODY()

    /* DataTable por defecto (si no hay variante) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* DefaultDialogue = nullptr;

    /* Variantes opcionales: A, B, C... */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, UDataTable*> VariantDialogues;

    /* ¿Puede repetirse si no se completa? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRepeatUntilCompleted = true;
};

/**
 * Mapa completo de un NPC:
 * Escena --> Entradas de diálogo
 */
USTRUCT(BlueprintType)
struct FNPCDialogueMap
{
    GENERATED_BODY()

    /* Escena --> diálogo */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, FDialogueSceneEntry> SceneDialogue;
};

/**
 * SUBSYSTEM PRINCIPAL DE HISTORIA
 */
UCLASS()
class AZULPROJECT_API UAzulStoryManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    /* ESCENA ACTUAL GLOBAL (1, 2, 3, 4...) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    int32 CurrentStoryScene = 1;

    /* VARIANTE ACTUAL DE LA ESCENA (A, B, C...) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    FName CurrentSceneVariant;

    /* FLAGS GLOBALES (decisiones importantes) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    TMap<FName, bool> GlobalFlags;

    /* TODA LA CONFIGURACIÓN DE DIÁLOGOS por NPC */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    TMap<FName, FNPCDialogueMap> DialogueMappings;

    /* Progreso de cada NPC: si ya completó su diálogo en la escena */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Story")
    TMap<FName, bool> NPCCompletedDialogue;

public:

    /* Devuelve la DataTable correcta para este NPC según la escena y variante */
    UFUNCTION(BlueprintCallable)
    UDataTable* GetDialogueForNPC(FName NPC_ID);

    /* Llamado al terminar un diálogo */
    UFUNCTION(BlueprintCallable)
    void OnNPCDialogueFinished(FName NPC_ID);

    /* Cambiar de escena */
    UFUNCTION(BlueprintCallable)
    void AdvanceScene(int32 NewScene, FName NewVariant = NAME_None);

    UFUNCTION(BlueprintCallable, Category = "Story")
    void ClearMappings();

    UFUNCTION(BlueprintCallable, Category = "Story")
    void AddNPCDialogueEntry(
        FName NPC_ID,
        int32 Scene,
        UDataTable* DefaultTable,
        TMap<FName, UDataTable*> VariantTables,
        bool bRepeatUntilCompleted
    );

    virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

};
