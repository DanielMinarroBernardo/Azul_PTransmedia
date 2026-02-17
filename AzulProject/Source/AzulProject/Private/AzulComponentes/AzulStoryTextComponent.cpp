#include "AzulComponentes/AzulStoryTextComponent.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Engine/DataTable.h"
#include "AzulSubsystem/AzulTutorialSubsystem.h"
#include "AzulSubsystem/AzulGameSubsystem.h"

UAzulStoryTextComponent::UAzulStoryTextComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    //DefaultStoryText = FText::FromString(TEXT("..."));
}

//void UAzulStoryTextComponent::BeginPlay()
//{
//    Super::BeginPlay();
//}
//
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

//CONTINUAR HACIENDO!

//void UAzulStoryTextComponent::SetTextStoryFromGameplay(const FGameplayTag& Gameplay)
//{
//    // Obtener el owner y castear a InteractuableBase
//    AAzulInteractuableBase* OwnerInteractuable = Cast<AAzulInteractuableBase>(GetOwner());
//    if (!OwnerInteractuable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Owner no es InteractuableBase"));
//        return;
//    }
//
//    // Obtener el Character
//    AAzulCharacterBase* Character = nullptr;
//    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
//    {
//        Character = Cast<AAzulCharacterBase>(PC->GetPawn());
//    }
//
//    if (!Character)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No hay CharacterBase asociado"));
//        return;
//    }
//
//    // Convertir tag a string
//    FString TagStr = Gameplay.GetTagName().ToString();
//
//    // Validar tabla
//    if (!StoryDataTable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("StoryDataTable no asignada"));
//        return;
//    }
//
//    // Obtener todas las filas de la DataTable
//    TArray<FStoryRow*> Rows;
//    StoryDataTable->GetAllRows<FStoryRow>(TEXT("SetTextStoryFromGameplay"), Rows);
//    if (Rows.Num() == 0)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No hay filas en StoryDataTable"));
//        return;
//    }
//
//    // Determinar índice según el tag
//    int32 Index = 0; // Default: Gameplay.01 → primer elemento
//    bool bIsGameplay1 = (TagStr == TEXT("Gameplay.01"));
//
//    if (!bIsGameplay1)
//    {
//        if (!TagStr.StartsWith(TEXT("Gameplay.")))
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Tag %s no es Gameplay.xx"), *TagStr);
//            return;
//        }
//
//        FString NumberStr = TagStr.RightChop(FString(TEXT("Gameplay.")).Len());
//        Index = FCString::Atoi(*NumberStr) - 1;
//
//        if (Index < 0 || Index >= Rows.Num())
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Índice %d fuera de rango en StoryDataTable"), Index);
//            return;
//        }
//    }
//
//    // Obtener texto correspondiente
//    FText StoryText = Rows[Index]->StoryText;
//
//    if (bIsGameplay1)
//    {
//        // 🔹 Setear tutorial text usando AzulTutorialSubsystem
//        if (UWorld* World = GetWorld())
//        {
//            if (UAzulTutorialSubsystem* TutorialSubsystem = World->GetSubsystem<UAzulTutorialSubsystem>())
//            {
//                TutorialSubsystem->SetTutorialText(StoryText);
//                UE_LOG(LogTemp, Log, TEXT("TutorialText seteado: %s"), *StoryText.ToString());
//            }
//        }
//    }
//    else
//    {
//        // 🔹 Setear texto en el HUD del player usando AzulGameSubsystem
//        if (UWorld* World = GetWorld())
//        {
//            if (UAzulGameSubsystem* GameSubsystem = World->GetSubsystem<UAzulGameSubsystem>())
//            {
//                // Por ahora guardamos en CurrentStoryText, luego se implementa función en el subsystem
//                CurrentStoryText = StoryText;
//                UE_LOG(LogTemp, Log, TEXT("GameStoryText seteado en subsystem: %s"), *StoryText.ToString());
//            }
//        }
//    }
//}




