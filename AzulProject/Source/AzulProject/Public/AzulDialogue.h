#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "AzulDialogue.generated.h"

// --- Estructuras auxiliares ---

USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Text;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 NextID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Score;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 ID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Type;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Speaker;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Text;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 NextID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FDialogueChoice> Choices;
};

USTRUCT(BlueprintType)
struct FDialogueScene
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 SceneID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString SceneName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FDialogueNode> Dialogues;
};

// --- Clase principal simplificada ---

UCLASS(Blueprintable, BlueprintType)
class AZULPROJECT_API UAzulDialogue : public UObject
{
    GENERATED_BODY()

public:
    UAzulDialogue();

protected:
    UPROPERTY(BlueprintReadOnly)
    TArray<FDialogueScene> Scenes;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentSceneIndex;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentDialogueID;

    UPROPERTY(BlueprintReadOnly)
    int32 PlayerScore;

public:

    /** Cargar datos desde el archivo JSON */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    bool LoadDialogueJSON();

    /** Iniciar una escena concreta */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void StartScene(int32 SceneIndex);

    /** Avanzar el diálogo al siguiente ID */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void AdvanceDialogue(int32 NextID);

    /** Escoger una opción */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void ChooseOption(int32 ChoiceIndex);

    /** Obtener texto actual */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    FString GetCurrentDialogueText() const;

    /** Obtener escena por índice */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    FDialogueScene GetSceneByIndex(int32 SceneIndex) const;

    /** Actualizar botones según las decisiones disponibles */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue|UI")
    void UpdateDecisionButtons(UHorizontalBox* ChoicesContainer);

    /** Manejar la selección de un botón */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue|UI")
    void HandleChoiceSelection(int32 ChoiceIndex, UHorizontalBox* ChoicesContainer);
};
