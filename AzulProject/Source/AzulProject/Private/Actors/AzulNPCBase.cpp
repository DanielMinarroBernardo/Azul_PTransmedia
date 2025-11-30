#include "Actors/AzulNPCBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Dialogos/AzulDialogueMappingDataAsset.h"
#include "GameplayTagContainer.h"
#include "Dialogos/AzulWidgetDialogueBase.h"
#include "Characters/AzulCharacterBase.h"


AAzulNPCBase::AAzulNPCBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAzulNPCBase::BeginPlay()
{
    Super::BeginPlay();

    // Crear motor de diálogo
    DialogueSystem = NewObject<UAzulDialogue>(this, UAzulDialogue::StaticClass());

    if (!DialogueSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo crear DialogueSystem en %s"), *GetName());
        return;
    }
}

void AAzulNPCBase::Interactua_Implementation()
{
    Super::Interactua_Implementation();


    if (!DialogueWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("NPC %s no tiene asignado un DialogueWidgetClass"), *GetName());
        return;
    }

    AAzulCharacterBase* Player = Cast<AAzulCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo obtener el PlayerCharacter"));
        return;
    }

    if (!DialogueMappingDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("NPC %s no tiene asignado DialogueMappingDataAsset"), *GetName());
        return;
    }

    UDataTable* TableToUse = DialogueMappingDataAsset->ResolveDialogueTable(NPCDialogueBaseTag, Player->ActiveStoryTags);

    UE_LOG(LogTemp, Warning, TEXT("TableToUse = %s"),
        TableToUse ? *TableToUse->GetName() : TEXT("NULL"));


    if (!TableToUse)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC %s no tiene diálogo válido para los tags actuales"), *GetName());
        return;
    }


    // Si este NPC no tiene diálogo en esta escena → NO habla
    if (!TableToUse)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC %s no tiene diálogo en la escena actual"), *GetName());
        return;
    }

    // 1. Crear el widget si no está
    if (!DialogueWidgetInstance)
    {
        UAzulWidgetDialogueBase* W = CreateWidget<UAzulWidgetDialogueBase>(GetWorld(), DialogueWidgetClass);
        W->NPC = this;
        DialogueWidgetInstance = W;
    }


    // 2. Añadirlo al viewport si no está visible
    if (!DialogueWidgetInstance->IsInViewport())
    {
        DialogueWidgetInstance->AddToViewport();
    }

    // 3. Cambiar input a UI Only
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(DialogueWidgetInstance->TakeWidget());
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }

    // 4. Reset delegates para evitar duplicados
    DialogueSystem->OnDialogueUpdated.RemoveDynamic(this, &AAzulNPCBase::OnNPCDialogueUpdated);
    DialogueSystem->OnDialogueFinished.RemoveDynamic(this, &AAzulNPCBase::OnNPCDialogueFinished);


    // 5. Conectar delegates
    DialogueSystem->OnDialogueUpdated.AddDynamic(this, &AAzulNPCBase::OnNPCDialogueUpdated);
    DialogueSystem->OnDialogueFinished.AddDynamic(this, &AAzulNPCBase::OnNPCDialogueFinished);


	// 6. Iniciar diálogo
    DialogueSystem->StartDialogue(TableToUse);

    
}

void AAzulNPCBase::OnNPCDialogueUpdated()
{
    if (!DialogueWidgetInstance) return;

    UTextBlock* Text = Cast<UTextBlock>(DialogueWidgetInstance->GetWidgetFromName(TEXT("DialogueText")));
    UHorizontalBox* Choices = Cast<UHorizontalBox>(DialogueWidgetInstance->GetWidgetFromName(TEXT("ChoicesContainer")));

    if (DialogueSystem)
    {
        DialogueSystem->SetDialogueText(Text);
        DialogueSystem->UpdateWidget(Choices);
    }
}

void AAzulNPCBase::OnNPCDialogueFinished()
{
    if (!DialogueWidgetInstance) return;

    DialogueWidgetInstance->RemoveFromParent();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void AAzulNPCBase::OnContinueRequested()
{
    if (DialogueSystem)
    {
        DialogueSystem->ContinueDialogue();
    }
}

void AAzulNPCBase::OnChoiceSelected(int32 Index)
{
    if (DialogueSystem)
    {
        DialogueSystem->OnChoiceClicked(Index);
    }
}
