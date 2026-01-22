// Fill out your copyright notice in the Description page of Project Settings.


#include "AzulComponentes/AzulStoryTextComponent.h"

UAzulStoryTextComponent::UAzulStoryTextComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    DefaultStoryText = FText::FromString(TEXT("..."));
}

void UAzulStoryTextComponent::BeginPlay()
{
    Super::BeginPlay();
}

FText UAzulStoryTextComponent::GetStoryTextForContext(FGameplayTag ContextTag) const
{
    if (StoryTextByContext.Contains(ContextTag))
    {
        return StoryTextByContext[ContextTag];
    }

    return DefaultStoryText;
}

bool UAzulStoryTextComponent::HasStoryTextForContext(FGameplayTag ContextTag) const
{
    return StoryTextByContext.Contains(ContextTag);
}

