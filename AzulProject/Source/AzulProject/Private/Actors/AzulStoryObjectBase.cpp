// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AzulStoryObjectBase.h"
#include "AzulStoryManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/AzulCharacterBase.h"


void AAzulStoryObjectBase::Interactua_Implementation()
{
    UE_LOG(LogTemp, Error, TEXT("STORYOBJECT: ¡Interactua se llamó correctamente!"));
    Super::Interactua_Implementation();

    // Obtener el PlayerCharacter
    AAzulCharacterBase* Player = Cast<AAzulCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("StoryObject: No se pudo obtener el PlayerCharacter"));
        return;
    }

    for (const FGameplayTag& TagToRemove : TagsToRemove)
    {
        if (Player->ActiveStoryTags.HasTag(TagToRemove))
        {
            Player->ActiveStoryTags.RemoveTag(TagToRemove);

            UE_LOG(LogTemp, Warning,
                TEXT("StoryObject %s ELIMINÓ TAG DEL PLAYER: %s"),
                *GetName(), *TagToRemove.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning,
                TEXT("StoryObject %s intentó eliminar TAG %s, pero el jugador NO lo tiene"),
                *GetName(), *TagToRemove.ToString());
        }
    }

    // Comprobar que VariantToSet no está vacío
    if (!VariantToSet.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("StoryObject %s: VariantToSet NO asignado"),
            *GetName());
        return;
    }

    // AÑADIR EL TAG AL PLAYER
    Player->ActiveStoryTags.AddTag(VariantToSet);

    UE_LOG(LogTemp, Warning, TEXT("StoryObject %s AÑADIÓ TAG: %s"),
        *GetName(), *VariantToSet.ToString());

    MeshComp->SetVisibility(false, true);

    OnInteracted();
}


