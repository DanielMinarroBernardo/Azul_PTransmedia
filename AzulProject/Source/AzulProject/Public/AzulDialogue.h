#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "AzulDialogue.generated.h"

USTRUCT(BlueprintType)
struct FDialogueRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsDecision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NextID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChoicesText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ChoicesNext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ChoicesScore;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS(Blueprintable, BlueprintType)
class AZULPROJECT_API UAzulDialogue : public UObject
{
    GENERATED_BODY()

public:

    /** DataTables que me pasas en orden */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UDataTable*> DialogueTables;

    /** Índice de la DataTable actual */
    int32 CurrentTableIndex = 0;

    /** ID actual del diálogo (Row Name → ID) */
    int32 CurrentID = 1;

    /** Row actual */
    FDialogueRow* CurrentRow = nullptr;

    /* Sistema de puntuación acumulada */
    UPROPERTY(BlueprintReadOnly)
    int32 PlayerScore = 0;

    /** Inicializa el sistema */
    UFUNCTION(BlueprintCallable)
    void StartDialogue();

    /** Obtiene la fila actual */
    bool LoadCurrentRow();

    /** Devuelve el texto actual */
    UFUNCTION(BlueprintCallable)
    FString GetCurrentText() const;

    /** Rellena los botones o activa botón Continuar */
    UFUNCTION(BlueprintCallable)
    void UpdateWidget(UHorizontalBox* ChoicesContainer);

    /** Avanza al siguiente ID si no hay decisiones */
    UFUNCTION(BlueprintCallable)
    void ContinueDialogue();

    /** Llamado desde los botones → selecciona opción */
    UFUNCTION(BlueprintCallable)
    void OnChoiceClicked(int32 ButtonIndex);
    
    /*Setea el current texto del diálogo*/
    UFUNCTION(BlueprintCallable)
    void SetDialogueText(UTextBlock* Text);

    UFUNCTION(BlueprintCallable)
    void SetDialogueTables(const TArray<UDataTable*>& InTables);



    /** Evento que el widget escucha para cerrarse */
    UPROPERTY(BlueprintAssignable)
    FOnDialogueFinished OnDialogueFinished;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueUpdated OnDialogueUpdated;

private:

    /** Variables auxiliares para manejar botones */
    UFUNCTION() void HandleContinueClicked();
    UFUNCTION() void HandleChoiceClicked();

    int32 PendingChoiceIndex = -1;

    UDataTable* CurrentTable = nullptr;
};
