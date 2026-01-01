#include "AzulSubsystem/AzulTutorialSubsystem.h"
#include "Components/HorizontalBox.h"
#include "Components/CheckBox.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Characters/AzulCharacterBase.h"
#include "Engine/Engine.h"

void UAzulTutorialSubsystem::NotifyActionCompleted(FGameplayTag ActionTag)
{
    if (!ActionTag.IsValid())
        return;

    if (CompletedActions.HasTag(ActionTag))
        return;

    CompletedActions.AddTag(ActionTag);

    OnTutorialStepUpdated.Broadcast(ActionTag, true);

    CheckSectionCompletion(ActionTag);
}



bool UAzulTutorialSubsystem::IsActionCompleted(FGameplayTag ActionTag) const
{
    return CompletedActions.HasTag(ActionTag);
}

bool UAzulTutorialSubsystem::IsSectionCompleted(FGameplayTag SectionTag) const
{
    return CompletedSections.HasTag(SectionTag);
}

void UAzulTutorialSubsystem::CheckSectionCompletion(FGameplayTag ActionTag)
{
    if (
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Space")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.W")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.A")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.S")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.D")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Look"))
        )
    {
        if (
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Space")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.W")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.A")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.S")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move.D")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Look"))
            )
        {
            FGameplayTag SectionTag =
                FGameplayTag::RequestGameplayTag("Tutorial.First");

            if (!CompletedSections.HasTag(SectionTag))
            {
                CompletedSections.AddTag(SectionTag);

                UE_LOG(LogTemp, Warning, TEXT("Tutorial section completed: %s"), *SectionTag.ToString());

                OnTutorialSectionCompleted.Broadcast(SectionTag);
            }
        }
    }
}

void UAzulTutorialSubsystem::StartTutorial()
{
    if (bTutorialActive)
        return;

    bTutorialActive = true;

    UE_LOG(LogTemp, Warning, TEXT("Tutorial STARTED"));
}

bool UAzulTutorialSubsystem::IsTutorialActive() const
{
    return bTutorialActive;
}
