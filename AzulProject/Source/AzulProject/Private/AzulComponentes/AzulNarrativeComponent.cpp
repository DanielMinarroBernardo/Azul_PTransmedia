// Fill out your copyright notice in the Description page of Project Settings.


#include "AzulComponentes/AzulNarrativeComponent.h"
#include "Dialogos/AzulDialogue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "AzulGameSubsystem.h"

// Sets default values for this component's properties
UAzulNarrativeComponent::UAzulNarrativeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAzulNarrativeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAzulNarrativeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAzulNarrativeComponent::StartNarrative()
{
    if (bUseDialogueSystem)
    {
        StartDialogueTable();
    }
    else
    {
        StartSimpleText();
    }
}

void UAzulNarrativeComponent::StartDialogueTable()
{
    if (!DialogueTable || !DialogueWidgetClass)
        return;

    DialogueInstance = NewObject<UAzulDialogue>(this);

    DialogueInstance->StartDialogue(DialogueTable);

    if (bShowDialogueWidget)
    {
        WidgetInstance = CreateWidget<UUserWidget>(
            GetWorld(),
            DialogueWidgetClass
        );
        WidgetInstance->AddToViewport();
    }

    if (UAzulGameSubsystem* NS =
        GetWorld()->GetGameInstance()->GetSubsystem<UAzulGameSubsystem>())
    {
        NS->RegisterDialogue(DialogueInstance);
    }
}

void UAzulNarrativeComponent::StartSimpleText()
{
    if (!DialogueWidgetClass || SimpleText.IsEmpty())
        return;

    WidgetInstance = CreateWidget<UUserWidget>(
        GetWorld(),
        DialogueWidgetClass
    );
    WidgetInstance->AddToViewport();
}

void UAzulNarrativeComponent::EndNarrative()
{
    if (WidgetInstance)
    {
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;
    }

    if (UAzulGameSubsystem* NS =
        GetWorld()->GetGameInstance()->GetSubsystem<UAzulGameSubsystem>())
    {
        NS->ClearDialogue();
    }
}
