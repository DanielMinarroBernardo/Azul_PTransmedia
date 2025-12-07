#include "Characters/AzulCharacterBase.h"
#include "Actors/AzulStoryObjectBase.h"
#include "Widgets/AzulWidgetBolsoBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Actors/AzulAscensorBase.h"

// Sets default values
AAzulCharacterBase::AAzulCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAzulCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    Bolso.SetNum(2);
    InitializeBolso();

    if (BolsoWidgetClass)
    {
        BolsoWidgetInstance = CreateWidget<UAzulWidgetBolsoBase>(GetWorld(), BolsoWidgetClass);
        if (!BolsoWidgetInstance->IsInViewport())
        {
            BolsoWidgetInstance->AddToViewport();
        }

        BolsoWidgetInstance->OwnerCharacter = this;

        BolsoWidgetInstance->SlotItems.SetNum(2);
        BolsoWidgetInstance->UpdateUI();
    }
}

// Called every frame
void AAzulCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAzulCharacterBase::AddStoryTag(const FGameplayTag& NewTag)
{
    ActiveStoryTags.AddTag(NewTag);
}

void AAzulCharacterBase::InitializeBolso()
{
    if (BolsoWidgetInstance)
    {
        BolsoWidgetInstance->ShowFullBolsoDialog();
    }

    Bolso[0] = FItemData();
    Bolso[1] = FItemData();
}

bool AAzulCharacterBase::TryAddItem(AAzulStoryObjectBase* WorldItem)
{
    UE_LOG(LogTemp, Error, TEXT("TryAddItem: WorldItem = %s"), *GetNameSafe(WorldItem));

    if (!WorldItem) return false;

    // Crear FItemData del nuevo objeto
    FItemData NewItem;
    NewItem.Mesh = WorldItem->MeshComp->GetStaticMesh();
    NewItem.Icon = WorldItem->Icon;

    // Buscar hueco vacío
    for (int i = 0; i < Bolso.Num(); i++)
    {
        if (!Bolso[i].IsValid())
        {
            Bolso[i] = NewItem;

            // Guarda el objeto físico para destruirlo DESPUÉS
            PendingPhysicalPick = WorldItem;
            PendingPhysicalPick->Destroy();

            BolsoWidgetInstance->SlotItems[i] = Bolso[i];
            BolsoWidgetInstance->UpdateUI();

            if (Bolso.Num() == 2) {
                CheckMision(8);
            }
            return true;
        }
    }

    // --- INVENTARIO LLENO → preparar swap ---
    PendingBolsoItem = NewItem;
    PendingBolsoItemWorldLocation = WorldItem->GetActorLocation();
    PendingPhysicalPick = WorldItem; // este se destruye al confirmar

    UE_LOG(LogTemp, Warning, TEXT("TryAddItem: Preparando swap, objeto físico = %s"), *GetNameSafe(WorldItem));

    WorldItem->SetActorEnableCollision(false);
    WorldItem->SetActorTickEnabled(false);

    BolsoWidgetInstance->ShowFullBolsoDialog();
    ActivateUIMode();

    return false;
}




void AAzulCharacterBase::HandleSwapConfirmed(int32 SlotIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("---- SWAP DEBUG ----"));
    UE_LOG(LogTemp, Warning, TEXT("OldItem Mesh: %s"), *GetNameSafe(Bolso[SlotIndex].Mesh));
    UE_LOG(LogTemp, Warning, TEXT("NewItem Mesh: %s"), *GetNameSafe(PendingBolsoItem.Mesh));
    UE_LOG(LogTemp, Warning, TEXT("PendingPhysicalPick: %s"), *GetNameSafe(PendingPhysicalPick));

    UE_LOG(LogTemp, Error, TEXT("HandleSwapConfirmed Slot=%d"), SlotIndex);

    if (!PendingBolsoItem.IsValid() || SlotIndex < 0)
        return;

    // 1. Guardar el item antiguo ANTES de reemplazarlo
    FItemData OldItem = Bolso[SlotIndex];

    // 2. Guardar ubicación donde se soltará el viejo
    FVector DropLocation = PendingBolsoItemWorldLocation;

    // 3. Colocar el nuevo item en el bolso
    Bolso[SlotIndex] = PendingBolsoItem;

    // 4. Actualizar también los datos del widget ANTES del UpdateUI
    if (BolsoWidgetInstance)
    {
        BolsoWidgetInstance->SlotItems[SlotIndex] = Bolso[SlotIndex];
    }

    // 5. Destruir SOLO una vez el objeto físico recién recogido
    if (PendingPhysicalPick && !PendingPhysicalPick->IsPendingKillPending())
    {
        PendingPhysicalPick->Destroy();
    }
    PendingPhysicalPick = nullptr;

    // 6. Soltar el objeto antiguo usando su mesh e icono correctos
    SpawnDroppedItem(OldItem, DropLocation);

    // 7. Limpiar datos temporales del swap
    PendingBolsoItem = FItemData();

    // 8. Actualizar interfaz del bolso
    BolsoWidgetInstance->ResetBolsoLayout();
    BolsoWidgetInstance->UpdateUI();

    // 9. Salir del modo UI
    DeactivateUIMode();

    UE_LOG(LogTemp, Warning, TEXT("Swap finalizado correctamente"));
}




void AAzulCharacterBase::SpawnDroppedItem(const FItemData& Item, const FVector& Location)
{
    if (BolsoWidgetInstance)
    {
        BolsoWidgetInstance->ShowFullBolsoDialog();
    }

    
    if (!Item.IsValid() || !ItemActorClass) return;

    FActorSpawnParameters Params;

    AAzulStoryObjectBase* Spawned =
        GetWorld()->SpawnActor<AAzulStoryObjectBase>(ItemActorClass, Location, FRotator::ZeroRotator);

    if (Spawned)
    {
        Spawned->MeshComp->SetStaticMesh(Item.Mesh);
        UE_LOG(LogTemp, Warning, TEXT("Dropped item spawned with mesh: %s"),
            *GetNameSafe(Item.Mesh));

        Spawned->Icon = Item.Icon;
    }
}

bool AAzulCharacterBase::IsLookingAtInteractable(UCameraComponent* Camera, float MinDot) const
{

    if (!Camera || !CurrentInteractable.GetObject())
        return false;

    AActor* RawActor = Cast<AActor>(CurrentInteractable.GetObject());
    if (!RawActor)
        return false;

    if (CurrentInteractableClass && CurrentInteractableClass->IsChildOf(AAzulAscensorBase::StaticClass()))
    {
        return true;
    }

    AAzulInteractuableBase* Interactable = Cast<AAzulInteractuableBase>(RawActor);
    if (!Interactable)
        return false;

    FVector TargetPoint = Interactable->MeshComp
        ? Interactable->MeshComp->Bounds.Origin
        : Interactable->GetActorLocation();

    FVector ToTarget = (TargetPoint - Camera->GetComponentLocation()).GetSafeNormal();
    float Dot = FVector::DotProduct(Camera->GetForwardVector(), ToTarget);

    return Dot >= MinDot;
}

void AAzulCharacterBase::ActivateUIMode()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void AAzulCharacterBase::DeactivateUIMode()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void AAzulCharacterBase::EnableMappingContext(FName Name, int32 Priority)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    ULocalPlayer* LP = PC->GetLocalPlayer();
    if (!LP) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    if (MappingContexts.Contains(Name))
    {
        Subsystem->AddMappingContext(MappingContexts[Name], Priority);
    }
}



void AAzulCharacterBase::DisableMappingContext(FName Name)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    ULocalPlayer* LP = PC->GetLocalPlayer();
    if (!LP) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    if (MappingContexts.Contains(Name))
    {
        Subsystem->RemoveMappingContext(MappingContexts[Name]);
        UE_LOG(LogTemp, Warning, TEXT("Disabled IMC: %s"), *Name.ToString());
    }
}


// Input binding
void AAzulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
