#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "Widgets/AzulWidgetHUDPlayer.h"
#include "AzulGameSubsystem.generated.h"

class UAzulDialogue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLevelSequenceFinished,
    ULevelSequence*, Sequence
);

UCLASS()
class AZULPROJECT_API UAzulGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    //---------------------------------------------------DIALOGOS

    /* Diálogo actualmente activo (si lo hay) */
    UPROPERTY(BlueprintReadOnly, Category = "Azul|Narrative")
    UAzulDialogue* ActiveDialogue = nullptr;

    /* Registrar un diálogo como activo */
    UFUNCTION(BlueprintCallable, Category = "Azul|Narrative")
    void RegisterDialogue(UAzulDialogue* Dialogue);

    /* Finalizar narrativa activa */
    UFUNCTION(BlueprintCallable, Category = "Azul|Narrative")
    void ClearDialogue();

    /* Petición genérica de avance (input, tutorial, etc.) */
    UFUNCTION(BlueprintCallable, Category = "Azul|Narrative")
    void RequestAdvanceDialogue();

    //-----------------------------------------------------------CINEMÁTICAS

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayLevelSequence(
        ULevelSequence* Sequence,
        bool bRestoreControlOnFinish = true,
        bool bHideCharacterMesh = true
    );

    UPROPERTY(BlueprintAssignable, Category = "Azul|Cinematics")
    FOnLevelSequenceFinished OnLevelSequenceFinished;

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayVideo(UMediaPlayer* MediaPlayer);

    UFUNCTION()
    bool IsSequenceActive();

	//--------------------------WIDGET HUD PLAYER
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|HUD Player")
    UAzulWidgetHUDPlayer* WidgetHUDPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|HUD Player|Mirilla")
    bool bIsLookingItem = false;

    //-----------------------------STORY TEXTS PROPS
    UFUNCTION(BlueprintCallable, Category = "Azul|StoryProps")
    FName GetCurrentGameplayName();

    UFUNCTION(BlueprintCallable, Category = "Azul|StoryProps")
    bool IsGameGameplay();

private:

    UPROPERTY()
    ULevelSequencePlayer* SequencePlayer = nullptr;

    UPROPERTY()
    ULevelSequence* CurrentSequence = nullptr;

    UPROPERTY()
    UMediaPlayer* ActiveMediaPlayer = nullptr;

    UPROPERTY()
    bool bRestoreControlAfterSequence = true;

    UPROPERTY()
    bool bHideCharacterMeshDuringSequence = true;

    UPROPERTY()
    bool bEnableMoveLookAfterSequence = true;



    UFUNCTION()
    void OnSequenceFinished();

    UFUNCTION()
    void OnVideoFinished();
};
