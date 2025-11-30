// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AzulInteractuableBase.h"
#include "GameplayTagContainer.h"
#include "AzulStoryObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class AZULPROJECT_API AAzulStoryObjectBase : public AAzulInteractuableBase
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    FGameplayTag VariantToSet;


    /* Escena que este objeto activa (0 para no cambiar escena) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    int32 SceneToSet = 0;

    virtual void Interactua_Implementation() override;
	
};
