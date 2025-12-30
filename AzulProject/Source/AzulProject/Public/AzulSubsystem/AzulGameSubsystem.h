#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "AzulGameSubsystem.generated.h"

class UAzulDialogue;

UCLASS()
class AZULPROJECT_API UAzulGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

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

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayLevelSequence(
        ULevelSequence* Sequence,
        bool bRestoreControlOnFinish = true,
        bool bHideCharacterMesh = true
    );

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayVideo(UMediaPlayer* MediaPlayer);

private:

    UPROPERTY()
    ULevelSequencePlayer* SequencePlayer = nullptr;

    UPROPERTY()
    UMediaPlayer* ActiveMediaPlayer = nullptr;

    UPROPERTY()
    bool bRestoreControlAfterSequence = true;

    UPROPERTY()
    bool bHideCharacterMeshDuringSequence = true;

    UFUNCTION()
    void OnSequenceFinished();

    UFUNCTION()
    void OnVideoFinished();
};
