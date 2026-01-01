#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "AzulTutorialSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FTutorialStepUpdated,
    FGameplayTag, StepTag,
    bool, bCompleted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTutorialSectionCompleted,
    FGameplayTag, SectionTag
);

UCLASS()
class AZULPROJECT_API UAzulTutorialSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    /* Notifica que una acción del tutorial se ha realizado */
    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    void NotifyActionCompleted(FGameplayTag ActionTag);

    /* ¿Está una acción completada? */
    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    bool IsActionCompleted(FGameplayTag ActionTag) const;

    /* ¿Está una sección completa? */
    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    bool IsSectionCompleted(FGameplayTag SectionTag) const;

    /* Evento: un paso se completa */
    UPROPERTY(BlueprintAssignable)
    FTutorialStepUpdated OnTutorialStepUpdated;

    /* Evento: una sección se completa */
    UPROPERTY(BlueprintAssignable)
    FTutorialSectionCompleted OnTutorialSectionCompleted;

    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    void StartTutorial();

    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    bool IsTutorialActive() const;


private:

    UPROPERTY()
    bool bTutorialActive = false;

    /* Acciones individuales completadas */
    UPROPERTY()
    FGameplayTagContainer CompletedActions;

    /* Secciones completadas */
    UPROPERTY()
    FGameplayTagContainer CompletedSections;

    void CheckSectionCompletion(FGameplayTag ActionTag);

};
