#include "Actors/AzulNPCBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "AzulStoryManagerSubsystem.h"

AAzulNPCBase::AAzulNPCBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAzulNPCBase::BeginPlay()
{
    Super::BeginPlay();

    // Crear motor de diálogo (sin tabla todavía)
    DialogueSystem = NewObject<UAzulDialogue>(this, UAzulDialogue::StaticClass());

    if (!DialogueSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo crear DialogueSystem en %s"), *GetName());
        return;
    }

    // Cuando termine el diálogo, queremos recibir un aviso
    DialogueSystem->OnDialogueFinished.AddDynamic(this, &AAzulNPCBase::OnNPCDialogueFinished);
}

void AAzulNPCBase::Interactua_Implementation()
{
    Super::Interactua_Implementation();

    if (!DialogueWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("NPC %s no tiene asignado un DialogueWidgetClass"), *GetName());
        return;
    }

    // PEDIMOS al StoryManager qué tabla toca en este momento
    UAzulStoryManagerSubsystem* StoryManager =
        GetGameInstance()->GetSubsystem<UAzulStoryManagerSubsystem>();

    if (!StoryManager)
    {
        UE_LOG(LogTemp, Error, TEXT("StoryManager no encontrado"));
        return;
    }

    UDataTable* TableToUse = StoryManager->GetDialogueForNPC(NPC_ID);

    // Si este NPC no tiene diálogo en esta escena → NO habla
    if (!TableToUse)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC %s no tiene diálogo en la escena actual"), *GetName());
        return;
    }

    // Crear y mostrar el widget
    DialogueWidgetInstance = CreateWidget<UUserWidget>(
        GetWorld(),
        DialogueWidgetClass
    );

    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->AddToViewport();
    }

    // Iniciar el diálogo en el estado correcto de la historia
    DialogueSystem->StartDialogue(TableToUse, true);
}

void AAzulNPCBase::OnNPCDialogueFinished()
{
    // Cerramos widget si estuviera abierto
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->RemoveFromParent();
        DialogueWidgetInstance = nullptr;
    }

    // Avisamos al StoryManager que este NPC ha terminado su diálogo
    UAzulStoryManagerSubsystem* StoryManager =
        GetGameInstance()->GetSubsystem<UAzulStoryManagerSubsystem>();

    if (StoryManager)
    {
        StoryManager->OnNPCDialogueFinished(NPC_ID);
    }
}