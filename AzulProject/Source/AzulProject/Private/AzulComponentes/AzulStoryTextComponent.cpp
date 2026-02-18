#include "AzulComponentes/AzulStoryTextComponent.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Engine/DataTable.h"
#include "Widgets/AzulWidgetTutorial.h"
#include "AzulSubsystem/AzulGameSubsystem.h"

UAzulStoryTextComponent::UAzulStoryTextComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    //DefaultStoryText = FText::FromString(TEXT("..."));
}

void UAzulStoryTextComponent::BeginPlay()
{
    Super::BeginPlay();
}


//FText UAzulStoryTextComponent::GetStoryTextForContext(FGameplayTag ContextTag) const
//{
//    if (StoryTextByContext.Contains(ContextTag))
//    {
//        return StoryTextByContext[ContextTag];
//    }
//
//    return DefaultStoryText;
//}
//
//bool UAzulStoryTextComponent::HasStoryTextForContext(FGameplayTag ContextTag) const
//{
//    return StoryTextByContext.Contains(ContextTag);
//}


void UAzulStoryTextComponent::SetTextStoryFromGameplay(const FGameplayTag& Gameplay)
{
//    // 1️⃣ Obtener Owner
//    AAzulInteractuableBase* OwnerInteractuable = Cast<AAzulInteractuableBase>(GetOwner());
//    if (!OwnerInteractuable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Owner no es AAzulInteractuableBase"));
//        return;
//    }
//
//    // 2️⃣ Obtener Character
//    AAzulCharacterBase* Character = nullptr;
//    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
//    {
//        Character = Cast<AAzulCharacterBase>(PC->GetPawn());
//    }
//
//    if (!Character)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No hay AAzulCharacterBase asociado"));
//        return;
//    }
//
//    // 3️⃣ Validar DataTable
//    if (!StoryDataTable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("StoryDataTable no asignada"));
//        return;
//    }
//
//    // 4️⃣ Obtener nombre de fila directamente desde el tag
//    FName RowName = Gameplay.GetTagName();
//
//    // 5️⃣ Buscar fila directamente
//    static const FString ContextString(TEXT("SetTextStoryFromGameplay"));
//
//    FStoryRow* FoundRow = StoryDataTable->FindRow<FStoryRow>(RowName, ContextString);
//
//    if (!FoundRow)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No se encontró fila %s en StoryDataTable"), *RowName.ToString());
//        return;
//    }
//
//    FText StoryText = FoundRow->StoryText;
//
//    // 6️⃣ Caso especial Gameplay.01
//    if (RowName == FName("Gameplay.01"))
//    {
//        UAzulWidgetTutorial* TutorialWidget = Character->GetTutorialWidget();
//        if (TutorialWidget)
//        {
//            TutorialWidget->SetTutorialText(StoryText);
//            UE_LOG(LogTemp, Log, TEXT("Texto seteado en TutorialWidget: %s"), *StoryText.ToString());
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("TutorialWidget no encontrado en Character"));
//        }
//    }
//    else
//    {
//        // 🔹 Setear texto en el HUD usando el Subsystem
//        if (UWorld* World = GetWorld())
//        {
//            if (UAzulGameSubsystem* GameSubsystem = World->GetSubsystem<UAzulGameSubsystem>())
//            {
//                CurrentStoryText = StoryText;
//                UE_LOG(LogTemp, Log, TEXT("Texto seteado en Subsystem: %s"), *StoryText.ToString());
//            }
//        }
//    }
}





