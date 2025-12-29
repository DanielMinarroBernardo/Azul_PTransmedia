#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "AzulGameSubsystem.generated.h"


UCLASS()
class AZULPROJECT_API UAzulGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayLevelSequence(ULevelSequence* Sequence);

    UFUNCTION(BlueprintCallable, Category = "Azul|Cinematics")
    void PlayVideo(UMediaPlayer* MediaPlayer);

private:


    UPROPERTY()
    ULevelSequencePlayer* SequencePlayer = nullptr;

    UPROPERTY()
    UMediaPlayer* ActiveMediaPlayer = nullptr;

    UFUNCTION()
    void OnSequenceFinished();

    UFUNCTION()
    void OnVideoFinished();
};
