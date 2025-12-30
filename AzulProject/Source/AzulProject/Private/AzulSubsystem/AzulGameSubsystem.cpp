#include "AzulSubsystem/AzulGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/AzulCharacterBase.h"
#include "Dialogos/AzulDialogue.h"

DEFINE_LOG_CATEGORY_STATIC(LogAzulCinematics, Log, All);

void UAzulGameSubsystem::PlayLevelSequence(
    ULevelSequence* Sequence,
    bool bRestoreControlOnFinish,
    bool bHideCharacterMesh
)
{
    if (!Sequence) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // 🔹 Guardamos los valores recibidos desde Blueprint
    bRestoreControlAfterSequence = bRestoreControlOnFinish;
    bHideCharacterMeshDuringSequence = bHideCharacterMesh;

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        APawn* Pawn = PC->GetPawn();
        AAzulCharacterBase* Character = Pawn ? Cast<AAzulCharacterBase>(Pawn) : nullptr;

        if (!Character)
        {
            UE_LOG(
                LogAzulCinematics,
                Warning,
                TEXT("PlayLevelSequence: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn)
            );
            return;
        }

        // 🔒 Bloqueamos control
        Character->BlockPlayerControl();

        // 👻 Ocultamos mesh si procede
        if (bHideCharacterMeshDuringSequence)
        {
            if (USkeletalMeshComponent* Mesh = Character->GetMesh())
            {
                Mesh->SetVisibility(false, true);
            }
        }
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
            UE_LOG(
                LogAzulCinematics,
                Warning,
                TEXT("PlayVideo: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn)
            );
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
            UE_LOG(
                LogAzulCinematics,
                Warning,
                TEXT("OnSequenceFinished: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn)
            );
            return;
        }

        // Devolvemos control solo si procede
        if (bRestoreControlAfterSequence)
        {
            Character->UnblockPlayerControl();
        }

        // Volvemos a mostrar el mesh si se ocultó
        if (bHideCharacterMeshDuringSequence)
        {
            if (USkeletalMeshComponent* Mesh = Character->GetMesh())
            {
                Mesh->SetVisibility(true, true);
            }
        }
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
            UE_LOG(
                LogAzulCinematics,
                Warning,
                TEXT("OnVideoFinished: Pawn no es AAzulCharacterBase (%s)"),
                *GetNameSafe(Pawn)
            );
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

void UAzulGameSubsystem::RegisterDialogue(UAzulDialogue* Dialogue)
{
    ActiveDialogue = Dialogue;
}

void UAzulGameSubsystem::ClearDialogue()
{
    ActiveDialogue = nullptr;
}

void UAzulGameSubsystem::RequestAdvanceDialogue()
{
    if (!ActiveDialogue)
        return;

    ActiveDialogue->ContinueDialogue();
}