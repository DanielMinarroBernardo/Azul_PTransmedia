#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "AzulTutorialSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FTutorialStepUpdated,
    FGameplayTag, StepTag,
    bool, bCompleted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTutorialCompleted,
    FGameplayTag, CompletedTag
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

    /* ¿El tutorial ya ha terminado? */
    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    bool IsTutorialCompleted() const;

    /* Evento: un paso individual se completa */
    UPROPERTY(BlueprintAssignable)
    FTutorialStepUpdated OnTutorialStepUpdated;

    /* Evento: tutorial COMPLETADO */
    UPROPERTY(BlueprintAssignable)
    FTutorialCompleted OnTutorialCompleted;

    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    void StartTutorial();

    UFUNCTION(BlueprintCallable, Category = "Azul|Tutorial")
    bool IsTutorialActive() const;

protected:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:

    UPROPERTY()
    bool bTutorialActive = false;

    /* Acciones individuales completadas */
    UPROPERTY()
    FGameplayTagContainer CompletedActions;

    /* Tags necesarios para terminar el tutorial */
    UPROPERTY()
    FGameplayTagContainer TutorialCompletionRequirements;

    /* Flag interno de tutorial terminado */
    UPROPERTY()
    bool bTutorialCompleted = false;

    void CheckTutorialCompletion();
};
