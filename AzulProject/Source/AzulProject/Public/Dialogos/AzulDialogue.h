#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/HorizontalBox.h"
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

    /** SOLO UNA DataTable */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* DialogueTable;

    /** Secuencia opcional de tablas para encadenar */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UDataTable*> DialogueSequence;

    /** Índice actual en la secuencia */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 CurrentTableIndex = 0;

    /** Repetir tabla hasta terminarla correctamente */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRepeatUntilFinished = false;

    /** Control opcional de reinicio manual */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasFinishedTable = false;

    /** ID actual */
    int32 CurrentID = 1;

    /** Fila actual */
    FDialogueRow* CurrentRow = nullptr;

    /* Puntuación */
    UPROPERTY(BlueprintReadOnly)
    int32 PlayerScore = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UButton* ContinueButton;

    UPROPERTY()
    TArray<UButton*> ChoiceButtons;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString PlayerName;


    /** Inicializa diálogo (llamado al pulsar E) */
    UFUNCTION(BlueprintCallable)
    void StartDialogue(UDataTable* OverrideTable = nullptr, bool bRestart = true);

    void LoadCurrentRow();

    UFUNCTION(BlueprintCallable)
    FString GetCurrentText() const;

    UFUNCTION(BlueprintCallable)
    void UpdateWidget(UHorizontalBox* ChoicesContainer);

    UFUNCTION(BlueprintCallable)
    void ContinueDialogue();

    UFUNCTION(BlueprintCallable)
    void OnChoiceClicked(int32 ButtonIndex);

    UFUNCTION(BlueprintCallable)
    void SetDialogueText(UTextBlock* Text);

    /** Set DataTable si quieres hacerlo por código */
    UFUNCTION(BlueprintCallable)
    void SetDialogueTable(UDataTable* InTable);

    /** Secuencia automática */
    UFUNCTION(BlueprintCallable)
    void AdvanceToNextTable();

    UPROPERTY(BlueprintAssignable)
    FOnDialogueFinished OnDialogueFinished;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueUpdated OnDialogueUpdated;

private:

    UFUNCTION()
    void HandleContinueClicked();

    UDataTable* CurrentTable = nullptr;
};
