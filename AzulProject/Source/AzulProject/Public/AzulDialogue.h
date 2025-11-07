#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

// --- Delegados expuestos a Blueprint ---

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueUpdated, const FDialogueNode&, Dialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoicesCreated, const TArray<FDialogueChoice>&, Choices);

// --- Clase principal ---

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
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    bool LoadDialogueJSON();

    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    TArray<FDialogueNode> GetDialoguesByScene(int32 SceneIndex) const;

    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void StartScene(int32 SceneIndex);

    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void AdvanceDialogue(int32 NextID);

    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    void ChooseOption(int32 ChoiceIndex);

    /** Devuelve la cantidad de opciones del diálogo actual */
    UFUNCTION(BlueprintCallable, Category = "AzulDialogue")
    int32 GetCurrentChoicesCount() const;

    /** 🔹 Eventos expuestos a Blueprint */
    UPROPERTY(BlueprintAssignable, Category = "AzulDialogue")
    FOnDialogueUpdated OnDialogueUpdated;

    UPROPERTY(BlueprintAssignable, Category = "AzulDialogue")
    FOnChoicesCreated OnChoicesCreated;
};
