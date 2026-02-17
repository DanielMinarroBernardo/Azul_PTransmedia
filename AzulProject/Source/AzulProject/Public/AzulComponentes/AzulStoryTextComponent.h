// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AzulStoryTextComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZULPROJECT_API UAzulStoryTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAzulStoryTextComponent();

protected:

    virtual void BeginPlay() override;

    //// Mapa de contexto narrativo → texto
    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
    //TMap<FGameplayTag, FText> StoryTextByContext;

    //// Texto por defecto si no hay coincidencia
    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
    //FText DefaultStoryText;

public:

    ///**
    // * Devuelve el texto narrativo según el contexto actual
    // */
    //UFUNCTION(BlueprintCallable, Category = "Story")
    //FText GetStoryTextForContext(FGameplayTag ContextTag) const;

    ///**
    // * Comprueba si existe texto para un contexto concreto
    // */
    //UFUNCTION(BlueprintCallable, Category = "Story")
    //bool HasStoryTextForContext(FGameplayTag ContextTag) const;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
    UDataTable* StoryDataTable;

    UPROPERTY(BlueprintReadOnly, Category = "Story")
    FText CurrentStoryText;

    UFUNCTION(BlueprintCallable, Category = "Azul|StoryText")
    void SetTextStoryFromGameplay(FGameplayTag Gameplay);
};
