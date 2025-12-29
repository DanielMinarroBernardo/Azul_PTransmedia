#include "AzulGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/AzulCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogAzulCinematics, Log, All);

void UAzulGameSubsystem::PlayLevelSequence(ULevelSequence* Sequence)
{
    if (!Sequence) return;

    UWorld* World = GetWorld();
    if (!World) return;

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        APawn* Pawn = PC->GetPawn();
        AAzulCharacterBase* Character = Pawn ? Cast<AAzulCharacterBase>(Pawn) : nullptr;

        if (!Character)
        {
            UE_LOG(LogAzulCinematics, Warning,
                TEXT("PlayLevelSequence: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn));
            return;
        }

        Character->BlockPlayerControl();
    }

    ALevelSequenceActor* SequenceActor = nullptr;

    FMovieSceneSequencePlaybackSettings Settings;
    SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        World,
        Sequence,
        Settings,
        SequenceActor
    );

    if (SequencePlayer)
    {
        SequencePlayer->OnFinished.AddDynamic(
            this,
            &UAzulGameSubsystem::OnSequenceFinished
        );

        SequencePlayer->Play();
    }
}

void UAzulGameSubsystem::PlayVideo(UMediaPlayer* MediaPlayer)
{
    if (!MediaPlayer) return;

    UWorld* World = GetWorld();
    if (!World) return;

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        APawn* Pawn = PC->GetPawn();
        AAzulCharacterBase* Character = Pawn ? Cast<AAzulCharacterBase>(Pawn) : nullptr;

        if (!Character)
        {
            UE_LOG(LogAzulCinematics, Warning,
                TEXT("PlayVideo: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn));
            return;
        }

        Character->BlockPlayerControl();
    }

    ActiveMediaPlayer = MediaPlayer;

    ActiveMediaPlayer->OnEndReached.AddDynamic(
        this,
        &UAzulGameSubsystem::OnVideoFinished
    );

    ActiveMediaPlayer->Play();
}

void UAzulGameSubsystem::OnSequenceFinished()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        APawn* Pawn = PC->GetPawn();
        AAzulCharacterBase* Character = Pawn ? Cast<AAzulCharacterBase>(Pawn) : nullptr;

        if (!Character)
        {
            UE_LOG(LogAzulCinematics, Warning,
                TEXT("OnSequenceFinished: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn));
            return;
        }

        Character->UnblockPlayerControl();
    }

    SequencePlayer = nullptr;
}

void UAzulGameSubsystem::OnVideoFinished()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        APawn* Pawn = PC->GetPawn();
        AAzulCharacterBase* Character = Pawn ? Cast<AAzulCharacterBase>(Pawn) : nullptr;

        if (!Character)
        {
            UE_LOG(LogAzulCinematics, Warning,
                TEXT("OnVideoFinished: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn));
            return;
        }

        Character->UnblockPlayerControl();
    }

    if (ActiveMediaPlayer)
    {
        ActiveMediaPlayer->OnEndReached.RemoveDynamic(
            this,
            &UAzulGameSubsystem::OnVideoFinished
        );
    }

    ActiveMediaPlayer = nullptr;
}
