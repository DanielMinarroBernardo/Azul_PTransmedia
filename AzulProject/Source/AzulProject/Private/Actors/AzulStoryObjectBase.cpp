// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AzulStoryObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/AzulCharacterBase.h"


void AAzulStoryObjectBase::Interactua_Implementation()
{
    UE_LOG(LogTemp, Error, TEXT("STORYOBJECT: ¡Interactua se llamó correctamente!"));

    if (!OverlappingCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("StoryObject: No se pudo obtener el PlayerCharacter"));
        return;
    }

    const bool bIsBolsoItem = BolsoTag.MatchesTag(FGameplayTag::RequestGameplayTag("Azul.StoryObject.CanPickup"));

    if (bIsBolsoItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s ES un ITEM DE BOLSO → TryAddItem + Story Logic, SIN ocultar mesh"), *GetName());

        // Primero intentamos meterlo en el bolso (esto incluye swap)
        OverlappingCharacter->TryAddItem(this);

        // AHORA ejecutamos lógica de historia
        // --- Remover tags ---
        for (const FGameplayTag& TagToRemove : TagsToRemove)
        {
            OverlappingCharacter->ActiveStoryTags.RemoveTag(TagToRemove);
        }

        // --- Añadir variante ---
        if (VariantToSet.IsValid())
        {
            OverlappingCharacter->ActiveStoryTags.AddTag(VariantToSet);
        }

        // ❗ IMPORTANTE: NO ocultar mesh ni destruirlo
        // ❗ DEJAR visible el actor porque TryAddItem decide si ocultarlo temporalmente

        OnExtraInteractBP();
        return;
    }

    // ---------------------------------------------------------
    // 2️⃣ CASO: OBJETO DE HISTORIA (BolsoTag == None)
    // Debe ejecutar lógica historia y OCULTAR mesh
    // ---------------------------------------------------------
    UE_LOG(LogTemp, Warning, TEXT("%s ES un OBJETO DE HISTORIA"), *GetName());

    // --- Remover tags ---
    for (const FGameplayTag& TagToRemove : TagsToRemove)
    {
        OverlappingCharacter->ActiveStoryTags.RemoveTag(TagToRemove);
    }

    // --- Añadir variante ---
    if (VariantToSet.IsValid())
    {
        OverlappingCharacter->ActiveStoryTags.AddTag(VariantToSet);
    }

    // ❗ SOLO OBJETOS DE HISTORIA OCULTAN SU MESH
    MeshComp->SetVisibility(false, true);

    OnExtraInteractBP();
}


