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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS(Blueprintable, BlueprintType)
class AZULPROJECT_API UAzulDialogue : public UObject
{
    GENERATED_BODY()

public:
    UAzulDialogue();

protected:

    /** Tabla actual de diálogo (solo escena activa) */
    UPROPERTY()
    UDataTable* DialogueTable;

    /** Lista de escenas en orden */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<UDataTable*> SceneTables;

    /** Escena actual (índice dentro del array) */
    UPROPERTY()
    int32 CurrentSceneIndex;

    /** Estado interno de diálogo */
    int32 CurrentID;
    int32 PlayerScore;

    /** Estado de resultado */
    bool bSceneCompletedCorrectly;

protected:

    FDialogueRow* GetCurrentRow();

    /** Determina si el jugador supera la escena */
    bool EvaluateScene();

    /** Cambia internamente a la siguiente escena o repite */
    void AdvanceSceneLogic();

public:

    UFUNCTION(BlueprintCallable)
    void InitScenes(const TArray<UDataTable*>& Scenes);

    UFUNCTION(BlueprintCallable)
    void StartScene();  // siempre empieza en el ID = 1

    UFUNCTION(BlueprintCallable)
    FString GetDialogueText();

    UFUNCTION(BlueprintCallable)
    bool IsDecision();

    UFUNCTION(BlueprintCallable)
    TArray<FString> GetChoiceTexts();

    UFUNCTION(BlueprintCallable)
    void SelectChoice(int32 ChoiceIndex);

    UFUNCTION(BlueprintCallable)
    void Continue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue|UI")
    void GetButtonData(int32 ButtonIndex, bool& bIsVisible, FString& OutText);

    UFUNCTION(BlueprintCallable, Category = "Dialogue|UI")
    void OnButtonPressed(int32 ButtonIndex);

    UFUNCTION(BlueprintCallable, Category = "Dialogue|UI")
    void IsSceneFinished(bool& bIsFinished);

    UFUNCTION(BlueprintCallable, Category = "Dialogue|UI")
    void SetCurrentButtons(UHorizontalBox* ChoiceContainer);


    /** Evento que el widget escucha para cerrarse */
    UPROPERTY(BlueprintAssignable)
    FOnDialogueFinished OnDialogueFinished;
};
