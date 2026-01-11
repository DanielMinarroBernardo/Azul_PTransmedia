#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

// Widgets
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"

// Gameplay Tags
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

#include "AzulWidgetTutorial.generated.h"

UENUM()
enum class EInteractHelpState : uint8
{
    None,
    FirstSet,
    SecondSet
};


UCLASS()
class AZULPROJECT_API UAzulWidgetTutorial : public UUserWidget
{
    GENERATED_BODY()

public:

    /* Llamada desde el Event Dispatcher del Subsystem */
    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    void FirstPartTutorial(FGameplayTag StepTag, bool bCompleted);

protected:

    virtual void NativeConstruct() override;
    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;

    /* -------- WIDGETS -------- */

    UPROPERTY(meta = (BindWidget))
    UBorder* TutorialBorder;

    UPROPERTY(meta = (BindWidget))
    UBorder* TextBorder;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* TutorialVerticalBox;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* CheckBoxVerticalBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox_1;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox_2;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox_3;

    UPROPERTY(meta = (BindWidget))
    UButton* ContinueButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TutorialText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TareaText_1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TareaText_2;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TareaText_3;

    UPROPERTY()
    FString MainText;

    UPROPERTY(meta = (BindWidget))
    UButton* SkipButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InteractHelp_01;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InteractHelp_02;

    UPROPERTY(meta = (BindWidget))
    UImage* InteractHelp_00;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InteractHelp_04;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InteractHelp_05;

    UPROPERTY(meta = (BindWidget))
    UImage* InteractHelp_03;

    bool bSecondPartActivated = false;

    EInteractHelpState InteractHelpState = EInteractHelpState::None;

    UFUNCTION()
    void ClearTutorialText();

    UFUNCTION()
    void ShowSecondInteractHelpSet();

    UFUNCTION()
    void CloseAllInteractHelp();

    UFUNCTION()
    void OnSkipTutorialPressed();

    UFUNCTION()
    void OnContinueButtonPressed();

    UFUNCTION()
    void SetCheckBoxsForSecondPart();

    UFUNCTION(BlueprintCallable)
    void SetTutorialText(const FString& NewText);

    void ApplyTutorialText();

    void EnableContinueButton();

    void OpenInteractHelp();

    void CompleteTutorial(const FGameplayTag& Tag);

    UFUNCTION()
    void HandleTutorialCompleted(FGameplayTag CompletedTag);

    FTimerHandle TextTimer;
    FTimerHandle ButtonTimer;

private:

    /* Acción actual que está esperando el tutorial */
    FGameplayTag CurrentStepTag;
};
