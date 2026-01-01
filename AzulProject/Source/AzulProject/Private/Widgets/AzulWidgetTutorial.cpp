#include "Widgets/AzulWidgetTutorial.h"
#include "GameFramework/PlayerController.h"
#include "Characters/AzulCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAzulWidgetTutorial::NativeConstruct()
{
    Super::NativeConstruct();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(
            this,
            &UAzulWidgetTutorial::OnContinueButtonPressed
        );

        ContinueButton->SetIsEnabled(false);
    }
}

void UAzulWidgetTutorial::FirstPartTutorial(FGameplayTag StepTag)
{
    if (!TutorialVerticalBox || !ContinueButton)
        return;

    CurrentStepTag = StepTag;

    // ---------- SPACE ----------
    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Space"))
    {
        CheckBox_1->SetIsChecked(true);

        ContinueButton->SetIsEnabled(true);
        return;
    }

    // ---------- MOVE (WASD) ----------
    if (
        StepTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move"))
        )
    {
        CheckBox_2->SetIsChecked(true);
        ContinueButton->SetIsEnabled(true);
        ContinueButton->GetParent()->SetIsEnabled(true);
        return;
    }

    // ---------- LOOK ----------
    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Look"))
    {
        CheckBox_3->SetIsChecked(true);
        ContinueButton->SetIsEnabled(true);
        return;
    }
}

void UAzulWidgetTutorial::OnContinueButtonPressed()
{
    if (!TutorialText || !ContinueButton)
        return;

    ContinueButton->SetIsEnabled(false);

    // --- FLUJO DE TEXTO ---
    if (CurrentStepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Space"))
    {
        TutorialText->SetText(
            FText::FromString(
                TEXT("Pulsa W para avanzar\nPulsa A y D para moverte lateralmente\nPulsa S para retroceder")
            )
        );


        CurrentStepTag = FGameplayTag::RequestGameplayTag("Tutorial.First.Move");

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn()))
            {
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
                }
            }
        }

        return;
    }

    if (CurrentStepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Move"))
    {
        TutorialText->SetText(
            FText::FromString(TEXT("Mueve el ratón para mirar alrededor"))
        );

        CurrentStepTag =
            FGameplayTag::RequestGameplayTag("Tutorial.First.Look");

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn()))
            {
                if (Character->GetCharacterMovement())
                {
                    Character->UnblockPlayerControl();
                }
            }
        }

        return;
    }
}
