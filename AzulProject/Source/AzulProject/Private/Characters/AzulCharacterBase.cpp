#include "Characters/AzulCharacterBase.h"
#include "Actors/AzulStoryObjectBase.h"
#include "Widgets/AzulWidgetBolsoBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Actors/AzulAscensorBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "AzulSubsystem/AzulTutorialSubsystem.h"
#include "AzulSubsystem/AzulGameSubsystem.h"
#include "GameplayTagContainer.h"


// Sets default values
AAzulCharacterBase::AAzulCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    BolsoComponent = CreateDefaultSubobject<UAzulBolsoComponent>(TEXT("BolsoComponent"));

    //HiloComponent = CreateDefaultSubobject<UAzulHiloComponent>(TEXT("HiloComponent"));

    CurrentInteractable = nullptr;
}

// Called when the game starts or when spawned
void AAzulCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    if (BolsoComponent)
    {
        BolsoComponent->InitializeBolso(this);
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


//------------------------------INPUT------------------------------

void AAzulCharacterBase::SetControlMode(EAzulControlMode NewMode)
{
    if (CurrentControlMode == NewMode)
        return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

    if (!InputSubsystem) return;

    // Quitar IMC anterior
    if (MappingContexts.Contains(CurrentControlMode))
    {
        InputSubsystem->RemoveMappingContext(
            MappingContexts[CurrentControlMode]
        );
    }

    // Añadir IMC nuevo
    if (MappingContexts.Contains(NewMode))
    {
        InputSubsystem->AddMappingContext(
            MappingContexts[NewMode],
            0
        );
    }

    CurrentControlMode = NewMode;
}


void AAzulCharacterBase::BlockPlayerControl()
{
    if (bIsBlocked) return;
    bIsBlocked = true;

    // 1️⃣ Bloquear movimiento
    GetCharacterMovement()->DisableMovement();

    // 2️⃣ Bloquear input
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetIgnoreLookInput(true); // Bloquea rotación

    //    DisableInput(PC);
    }
}

void AAzulCharacterBase::UnblockPlayerControl()
{
    if (!bIsBlocked) return;
    bIsBlocked = false;

    // 1Restaurar movimiento
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    // 2Restaurar input
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetIgnoreLookInput(false);
    //    EnableInput(PC);

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
}



void AAzulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent CALLED"));

     if (UEnhancedInputComponent* EIC =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent FOUND"));

        EIC->BindAction(
            IA_MostrarHilo,
            ETriggerEvent::Started,
            this,
            &AAzulCharacterBase::OnSpacePressed
        );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent NOT FOUND"));
    }


}



void AAzulCharacterBase::OnSpacePressed()
{
    //--- TUTORIAL
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UAzulTutorialSubsystem* TutorialSubsystem =
            GI->GetSubsystem<UAzulTutorialSubsystem>())
        {
            if (TutorialSubsystem->IsTutorialActive())
            {
                const FGameplayTag SpaceTag =
                    FGameplayTag::RequestGameplayTag(TEXT("Tutorial.First.Space"));

                if (!TutorialSubsystem->IsActionCompleted(SpaceTag)) {
                    TutorialSubsystem->NotifyActionCompleted(
                        FGameplayTag::RequestGameplayTag("Tutorial.First.Space")
                    );
                }
            }
        }
    }

    if (UGameInstance* GI = GetGameInstance())
    {
        if (UAzulGameSubsystem* GameSubsystem =
            GI->GetSubsystem<UAzulGameSubsystem>())
        {
            if (!GameSubsystem->IsSequenceActive())
            {
                // --- HILO ---
                if (!HiloActor)
                    return;

                if (HiloActor->IsHiloVisible())
                {
                    // Segundo espacio antes de 6s → ocultar
                    HiloActor->ForceHideHilo();
                    NotifyHiloHidden();
                }
                else
                {
                    // Primer espacio → mostrar
                    HiloActor->ShowHilo();
                    NotifyHiloShown();
                }
            }
        }
    }
}

//----------------------------------------------HILO

void AAzulCharacterBase::NotifyHiloShown()
{
    BP_OnHiloShown();
}

void AAzulCharacterBase::NotifyHiloHidden()
{
    BP_OnHiloHidden();
}

//------------------------------------MIRILLA

void AAzulCharacterBase::OpenMirilla()
{
    BP_OpenMirilla();
}



void AAzulCharacterBase::UpdatedMirillaUI(bool bInRange, bool bLooking)
{
    if (!HUDWidget) return;

    if (!bInRange)
    {
        HUDWidget->SetUIState(EInteractUIState::Default);
    }
    else if (bInRange && !bLooking)
    {
        HUDWidget->SetUIState(EInteractUIState::InRange);
    }
    else
    {
        HUDWidget->SetUIState(EInteractUIState::InRangeAndLooking);
    }
}

void AAzulCharacterBase::StartInteractTrace()
{
    UE_LOG(LogTemp, Warning, TEXT("[INTERACT] StartInteractTrace"));

    if (GetWorld()->GetTimerManager().IsTimerActive(InteractTraceTimer))
    {
        UE_LOG(LogTemp, Warning, TEXT("[INTERACT] Trace already active"));
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(
        InteractTraceTimer,
        this,
        &AAzulCharacterBase::PerformInteractTrace,
        InteractTraceInterval,
        true
    );
}


void AAzulCharacterBase::StopInteractTrace()
{
    UE_LOG(LogTemp, Warning, TEXT("[INTERACT] StopInteractTrace"));

    GetWorld()->GetTimerManager().ClearTimer(InteractTraceTimer);

    bCanInteract = false;
    UpdatedMirillaUI(false, false);
}


void AAzulCharacterBase::PerformInteractTrace()
{
    UE_LOG(LogTemp, Verbose, TEXT("[INTERACT] PerformInteractTrace"));

    // 1️⃣ Si no hay interactuables en rango, no tiene sentido trazar
    if (OverlappingInteractables.Num() == 0)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[INTERACT] No overlapping interactables"));

        CurrentInteractable = nullptr;
        bCanInteract = false;
        UpdatedMirillaUI(false, false);
        return;
    }

    // 2️⃣ Mirilla obligatoria
    if (!HUDWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("[INTERACT] MirillaWidget is NULL"));

        CurrentInteractable = nullptr;
        bCanInteract = false;
        return;
    }

    // 3️⃣ PlayerController necesario para el deproject
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
    {
        CurrentInteractable = nullptr;
        bCanInteract = false;
        UpdatedMirillaUI(true, false);
        return;
    }

    // 4️⃣ Obtener rayo desde el centro de la pantalla
    FVector WorldLocation;
    FVector WorldDirection;

    const FVector2D ViewportSize =
        FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    const FVector2D ScreenCenter(
        ViewportSize.X * 0.5f,
        ViewportSize.Y * 0.5f
    );

    if (!PC->DeprojectScreenPositionToWorld(
        ScreenCenter.X,
        ScreenCenter.Y,
        WorldLocation,
        WorldDirection))
    {
        CurrentInteractable = nullptr;
        bCanInteract = false;
        UpdatedMirillaUI(true, false);
        return;
    }

    FVector Start = WorldLocation;
    FVector End = Start + WorldDirection * InteractTraceDistance;

    // 5️⃣ LineTrace
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    //if (bHit && GEngine)
    //{
    //    FString Msg = FString::Printf(
    //        TEXT("HIT → Actor: %s | Comp: %s"),
    //        Hit.GetActor() ? *Hit.GetActor()->GetName() : TEXT("None"),
    //        Hit.GetComponent() ? *Hit.GetComponent()->GetName() : TEXT("None")
    //    );

    //    GEngine->AddOnScreenDebugMessage(
    //        -1,
    //        0.1f,
    //        FColor::Green,
    //        Msg
    //    );
    //}


    // 6️⃣ Buscar si el hit corresponde a ALGUNO de los interactuables en rango
    AAzulInteractuableBase* HitInteractable = nullptr;

    if (bHit)
    {
        for (const TScriptInterface<IAzulInteractuableInterface>& Interactable : OverlappingInteractables)
        {
            if (!Interactable.GetObject())
                continue;

            AAzulInteractuableBase* Candidate =
                Cast<AAzulInteractuableBase>(Interactable.GetObject());

            if (!Candidate || !Candidate->MeshComp)
                continue;

            if (Hit.GetComponent() == Candidate->MeshComp)
            {
                HitInteractable = Candidate;
                break;
            }
        }
    }

    // 7️⃣ Resultado final del sistema
    if (HitInteractable)
    {
        CurrentInteractable =
            TScriptInterface<IAzulInteractuableInterface>(HitInteractable);

        bCanInteract = true;
        UpdatedMirillaUI(true, true);

        UE_LOG(
            LogTemp,
            Verbose,
            TEXT("[INTERACT] Looking at interactable: %s"),
            *HitInteractable->GetName()
        );
    }
    else
    {
        CurrentInteractable = nullptr;
        bCanInteract = false;
        UpdatedMirillaUI(true, false);
    }
}


void AAzulCharacterBase::AddInteractable(
    TScriptInterface<IAzulInteractuableInterface> Interactable)
{
    if (!Interactable.GetObject())
        return;

    // --- LOG del nombre del interactuable ---
    UObject* InteractableObject = Interactable.GetObject();
    AActor* InteractableActor = Cast<AActor>(InteractableObject);

    if (InteractableActor)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Interactable added: %s"),
            *InteractableActor->GetName()
        );
    }
    else
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Interactable added (non-actor object): %s"),
            *InteractableObject->GetName()
        );
    }

    OverlappingInteractables.AddUnique(Interactable);

    if (OverlappingInteractables.Num() == 1)
    {
        StartInteractTrace();
    }
}


void AAzulCharacterBase::RemoveInteractable(
    TScriptInterface<IAzulInteractuableInterface> Interactable)
{
    if (!Interactable.GetObject())
        return;

    OverlappingInteractables.Remove(Interactable);

    if (OverlappingInteractables.Num() == 0)
    {
        CurrentInteractable = nullptr;
        StopInteractTrace();
    }
}



