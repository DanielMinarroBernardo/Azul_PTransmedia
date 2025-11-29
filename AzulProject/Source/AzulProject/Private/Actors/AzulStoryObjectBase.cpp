// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AzulStoryObjectBase.h"
#include "AzulStoryManagerSubsystem.h"

void AAzulStoryObjectBase::Interactua_Implementation()
{
    Super::Interactua_Implementation();

    UAzulStoryManagerSubsystem* Story =
        GetGameInstance()->GetSubsystem<UAzulStoryManagerSubsystem>();

    if (!Story)
        return;

    // Cambiar variante
    if (!VariantToSet.IsNone())
    {
        Story->CurrentSceneVariant = VariantToSet;
    }

    // Cambiar escena
    if (SceneToSet > 0)
    {
        Story->AdvanceScene(SceneToSet, Story->CurrentSceneVariant);
    }
}

