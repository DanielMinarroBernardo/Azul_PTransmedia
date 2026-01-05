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

        MainText = TEXT("This is the thread that will always take you to your child whenever you need it. It lasts 6 seconds, but you can remove it before that time by pressing the space bar again.");
        //Pasa 2 segundos y cambiamos texto
        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            1.0f,
            false
        );

        EnableContinueButton();

        return;
    }

    // ---------- MOVE (WASD) ----------
    if (
        StepTag.MatchesTag(FGameplayTag::RequestGameplayTag("Tutorial.First.Move"))
        )
    {
        CheckBox_2->SetIsChecked(true);

        MainText = TEXT("There you are, now you can move in all directions and explore every location.");

        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            1.0f,
            false
        );

        EnableContinueButton();

        return;
    }

    // ---------- LOOK ----------
    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Look"))
    {
        CheckBox_3->SetIsChecked(true);
        ContinueButton->SetVisibility(ESlateVisibility::Hidden);

        MainText = TEXT("Great, now you can see everything you want by turning your character's head.");

        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            1.0f,
            false
        );

        //Cambiar los checkbox después de delay
        GetWorld()->GetTimerManager().SetTimer(
            ButtonTimer,
            this,
            &UAzulWidgetTutorial::SetCheckBoxsForSecondPart,
            7.0f,
            false
        );



        return;
    }

    // ---------- INTERACTUAR ----------
    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.Interact"))
    {
        CheckBox_1->SetIsChecked(true);
        
        MainText = TEXT("Very well,");

        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            5.0f,
            false
        );

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
                TEXT("Press W to move forward\nPress A and D to move sideways\nPress S to move backward")
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
            FText::FromString(TEXT("Move your mouse to look around"))
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

void UAzulWidgetTutorial::SetCheckBoxsForSecondPart()
{
    ContinueButton->SetIsEnabled(false);

    if (!CheckBox_1 || !CheckBox_2 || !CheckBox_3)
        return;
    CheckBox_1->SetIsChecked(false);
    CheckBox_2->SetIsChecked(false);
	CheckBox_3->SetIsChecked(false);

    TareaText_1->SetText(FText::FromString(TEXT("Interact with any interactive object")));
    TareaText_2->SetText(FText::FromString(TEXT("Take the manual.")));
    TareaText_3->SetText(FText::FromString(TEXT("Open the manual")));


    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn()))
        {
            Character->UnblockPlayerControl();
            Character->OpenMirilla();
            FString SonNameString;

            // Si SonName es FString
            SonNameString = Character->SonName;

            // Si SonName es FName (usa esta línea en vez de la anterior)
            // SonNameString = Character->SonName.ToString();

            FString TutorialString = FString::Printf(
                TEXT("%s is crying, what could be wrong with him?"),
                *SonNameString
            );

            TutorialText->SetText(FText::FromString(TutorialString));

        }
    }
}

void UAzulWidgetTutorial::SetTutorialText(const FString& NewText)
{
    if (TutorialText)
    {
        TutorialText->SetText(FText::FromString(NewText));
    }
}

void UAzulWidgetTutorial::ApplyTutorialText()
{
    SetTutorialText(MainText);
}

void UAzulWidgetTutorial::EnableContinueButton()
{
    ContinueButton->SetIsEnabled(true);
}


