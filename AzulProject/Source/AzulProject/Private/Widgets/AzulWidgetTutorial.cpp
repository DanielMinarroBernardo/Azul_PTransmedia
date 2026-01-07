#include "Widgets/AzulWidgetTutorial.h"
#include "GameFramework/PlayerController.h"
#include "Characters/AzulCharacterBase.h"
#include "AzulSubsystem/AzulTutorialSubsystem.h"
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
    }

    if (SkipButton)
    {
        SkipButton->OnClicked.AddDynamic(
            this,
            &UAzulWidgetTutorial::OnSkipTutorialPressed
        );
    }


    ContinueButton->SetIsEnabled(false);

    CheckBox_1->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox_2->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox_3->SetVisibility(ESlateVisibility::HitTestInvisible);

    InteractHelp_00->SetVisibility(ESlateVisibility::Hidden);
    InteractHelp_01->SetVisibility(ESlateVisibility::Hidden);
    InteractHelp_02->SetVisibility(ESlateVisibility::Hidden);

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
        GetWorld()->GetTimerManager().ClearTimer(TextTimer);
        
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
        GetWorld()->GetTimerManager().ClearTimer(TextTimer);
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
        GetWorld()->GetTimerManager().ClearTimer(TextTimer);
        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            1.0f,
            false
        );

        GetWorld()->GetTimerManager().ClearTimer(ButtonTimer);
        //Cambiar los checkbox después de delay
        GetWorld()->GetTimerManager().SetTimer(
            ButtonTimer,
            this,
            &UAzulWidgetTutorial::SetCheckBoxsForSecondPart,
            3.0f,
            false
        );



        return;
    }

    // ---------- INTERACTUAR ----------
    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.Interact"))
    {
        CheckBox_1->SetIsChecked(true);
        
        //MainText = TEXT("Very well,");
        GetWorld()->GetTimerManager().ClearTimer(ButtonTimer);
        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            5.0f,
            false
        );

        return;
    }

    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.TakeManual")) {
        CheckBox_2->SetIsChecked(true);

        MainText = TEXT("Now open it.");
        GetWorld()->GetTimerManager().ClearTimer(ButtonTimer);
        GetWorld()->GetTimerManager().SetTimer(
            TextTimer,
            this,
            &UAzulWidgetTutorial::ApplyTutorialText,
            5.0f,
            false
        );

        return;
    }

    if (StepTag == FGameplayTag::RequestGameplayTag("Tutorial.OpenManual")) {
        CheckBox_3->SetIsChecked(true);

        MainText = TEXT("Yes");
        GetWorld()->GetTimerManager().ClearTimer(ButtonTimer);
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

        if (!GetGameInstance())
            return;

        UAzulTutorialSubsystem* TutorialSubsystem =
            GetGameInstance()->GetSubsystem<UAzulTutorialSubsystem>();

        if (!TutorialSubsystem)
            return;

        TutorialSubsystem->bCanLookTutorial = true;

        return;
    }

    if (CurrentStepTag == FGameplayTag::RequestGameplayTag("Tutorial.First.Look")) {
        InteractHelp_00->SetVisibility(ESlateVisibility::Hidden);
        InteractHelp_01->SetVisibility(ESlateVisibility::Hidden);
        InteractHelp_02->SetVisibility(ESlateVisibility::Hidden);

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn()))
            {
                // --- Desbloquear el look (cámara) ---
                PC->SetIgnoreLookInput(false);

                // --- Ocultar cursor ---
                PC->bShowMouseCursor = false;

                // --- Modo de input: solo juego ---
                FInputModeGameOnly InputMode;
                PC->SetInputMode(InputMode);

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
}

void UAzulWidgetTutorial::SetCheckBoxsForSecondPart()
{
    GetWorld()->GetTimerManager().ClearTimer(TextTimer);

    if (!CheckBox_1 || !CheckBox_2 || !CheckBox_3)
        return;
    CheckBox_1->SetIsChecked(false);
    CheckBox_2->SetIsChecked(false);
	CheckBox_3->SetIsChecked(false);

    TareaText_1->SetText(FText::FromString(TEXT("Interact with any interactive object")));
    TareaText_2->SetText(FText::FromString(TEXT("Take the manual.")));
    TareaText_3->SetText(FText::FromString(TEXT("Open the manual with the M key")));

    OpenInteractHelp();
    
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

void UAzulWidgetTutorial::OpenInteractHelp()
{
    // --- Mostrar ayudas ---
    if (InteractHelp_00)
        InteractHelp_00->SetVisibility(ESlateVisibility::Visible);

    if (InteractHelp_01)
        InteractHelp_01->SetVisibility(ESlateVisibility::Visible);

    if (InteractHelp_02)
        InteractHelp_02->SetVisibility(ESlateVisibility::Visible);

    // --- Obtener PlayerController ---
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
        return;

    // --- Bloquear el look (cámara) ---
    PC->SetIgnoreLookInput(true);

    // --- Mostrar cursor ---
    PC->bShowMouseCursor = true;

    // --- Modo de input: solo UI (o UI + Game si prefieres) ---
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    // --- Mostrar botón Continue ---
    if (ContinueButton)
    {
        ContinueButton->SetVisibility(ESlateVisibility::Visible);
        ContinueButton->SetIsEnabled(true);
    }
}


void UAzulWidgetTutorial::OnSkipTutorialPressed()
{
    // 1. Obtener el Subsystem
    UAzulTutorialSubsystem* TutorialSubsystem =
        GetGameInstance()->GetSubsystem<UAzulTutorialSubsystem>();


    if (!TutorialSubsystem)
        return;

    // 2. Marcar TODOS los pasos como completados
    TutorialSubsystem->NotifyActionCompleted(
        FGameplayTag::RequestGameplayTag("Tutorial.First.Space")
    );

    TutorialSubsystem->NotifyActionCompleted(
        FGameplayTag::RequestGameplayTag("Tutorial.First.Move")
    );

    TutorialSubsystem->NotifyActionCompleted(
        FGameplayTag::RequestGameplayTag("Tutorial.First.Look")
    );

    TutorialSubsystem->NotifyActionCompleted(
        FGameplayTag::RequestGameplayTag("Tutorial.Interact")
    );

    // 3. Desbloquear jugador y activar mirilla
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn()))
        {
            Character->UnblockPlayerControl();
            Character->OpenMirilla();

            if (Character->GetCharacterMovement())
            {
                Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }
        }
    }

    // 4. Ocultar el widget de tutorial
    RemoveFromParent();
}


