#include "AzulSubsystem/AzulTutorialSubsystem.h"

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

    if (ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Space")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.W")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.A")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.S")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.D")) ||
        ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Look")))
    {
        if (CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Space")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.W")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.A")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.S")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Move.D")) &&
            CompletedActions.HasTag(FGameplayTag::RequestGameplayTag("Tutorial.Basic.Look")))
        {
            FGameplayTag SectionTag = FGameplayTag::RequestGameplayTag("Tutorial.Basic");

            if (!CompletedSections.HasTag(SectionTag))
            {
                CompletedSections.AddTag(SectionTag);
                OnTutorialSectionCompleted.Broadcast(SectionTag);
            }
        }
    }
}