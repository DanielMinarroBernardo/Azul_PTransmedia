#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "AzulGameSubsystem.generated.h"


/**
 * SUBSYSTEM PRINCIPAL DE HISTORIA
 */
UCLASS()
class AZULPROJECT_API UAzulGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ---- API pública ----
    void PlayLevelSequence(ULevelSequence* Sequence, UObject* WorldContext);
    void PlayVideo(UMediaPlayer* MediaPlayer);

    bool IsCinematicPlaying() const { return bIsPlaying; }

    void ForceEndCinematic();

private:
    // ---- Estado interno ----
    bool bIsPlaying = false;

    UPROPERTY()
    ULevelSequencePlayer* ActiveSequencePlayer = nullptr;

    UPROPERTY()
    UMediaPlayer* ActiveMediaPlayer = nullptr;

    // ---- Flujo interno ----
    void EnterCinematicMode();
    void ExitCinematicMode();

    // ---- Callbacks ----
    UFUNCTION()
    void OnSequenceFinished();

    UFUNCTION()
    void OnVideoFinished();

    // ---- Utilidades ----
    APlayerController* GetPlayerController() const;
};
