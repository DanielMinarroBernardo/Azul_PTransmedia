// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogos/AzulWidgetDialogueBase.h"
#include "Actors/AzulNPCBase.h"

void UAzulWidgetDialogueBase::PressContinue()
{
    if (NPC)
    {
        NPC->OnContinueRequested();
    }
}

void UAzulWidgetDialogueBase::PressChoice(int32 ChoiceIndex)
{
    if (NPC)
        NPC->OnChoiceSelected(ChoiceIndex);
}
