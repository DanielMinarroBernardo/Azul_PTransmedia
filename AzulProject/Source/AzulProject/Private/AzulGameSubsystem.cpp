#include "AzulGameSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Media
#include "MediaPlayer.h"

// Level Sequence
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

void UAzulGameSubsystem::PlayLevelSequence(ULevelSequence* Sequence, UObject* WorldContext)
{
    if (!Sequence || bIsPlaying || !WorldContext)
    {
        return;
    }

    UWorld* World = WorldContext->GetWorld();
    if (!World)
    {
        return;
    }

    EnterCinematicMode();

    FMovieSceneSequencePlaybackSettings PlaybackSettings;
    PlaybackSettings.bAutoPlay = false;

    ALevelSequenceActor* SequenceActor = nullptr;

    ActiveSequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            World,
            Sequence,
            PlaybackSettings,
            SequenceActor
        );

    if (!ActiveSequencePlayer)
    {
        ExitCinematicMode();
        return;
    }

    bIsPlaying = true;

    ActiveSequencePlayer->OnFinished.AddDynamic(
        this,
        &UAzulGameSubsystem::OnSequenceFinished
    );

    ActiveSequencePlayer->Play();
}

void UAzulGameSubsystem::PlayVideo(UMediaPlayer* MediaPlayer)
{
    if (!MediaPlayer || bIsPlaying)
    {
        return;
    }

    EnterCinematicMode();

    ActiveMediaPlayer = MediaPlayer;
    bIsPlaying = true;

    ActiveMediaPlayer->OnEndReached.AddDynamic(
        this,
        &UAzulGameSubsystem::OnVideoFinished
    );

    ActiveMediaPlayer->Play();
}

void UAzulGameSubsystem::OnSequenceFinished()
{
    ExitCinematicMode();
}

void UAzulGameSubsystem::OnVideoFinished()
{
    ExitCinematicMode();
}

void UAzulGameSubsystem::ForceEndCinematic()
{
    if (!bIsPlaying)
    {
        return;
    }

    if (ActiveSequencePlayer)
    {
        ActiveSequencePlayer->Stop();
        ActiveSequencePlayer = nullptr;
    }

    if (ActiveMediaPlayer)
    {
        ActiveMediaPlayer->Close();
        ActiveMediaPlayer = nullptr;
    }

    ExitCinematicMode();
}

void UAzulGameSubsystem::EnterCinematicMode()
{
    APlayerController* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    PC->SetShowMouseCursor(false);
    PC->SetInputMode(FInputModeUIOnly());

    // OJO: aquí NO tocamos bIsPlaying todavía
}

void UAzulGameSubsystem::ExitCinematicMode()
{
    APlayerController* PC = GetPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    bIsPlaying = false;

    ActiveSequencePlayer = nullptr;
    ActiveMediaPlayer = nullptr;
}

APlayerController* UAzulGameSubsystem::GetPlayerController() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    return UGameplayStatics::GetPlayerController(World, 0);
}
