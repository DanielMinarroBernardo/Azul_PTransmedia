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
#include "EnhancedInputComponent.h"
#include "AzulSubsystem/AzulTutorialSubsystem.h"
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



bool AAzulCharacterBase::IsLookingAtInteractable(UCameraComponent* Camera) const
{
    if (!Camera || !CurrentInteractable.GetObject())
        return false;

    AActor* InteractableActor = Cast<AActor>(CurrentInteractable.GetObject());
    if (!InteractableActor)
        return false;

    AAzulInteractuableBase* Interactable =
        Cast<AAzulInteractuableBase>(InteractableActor);
    if (!Interactable || !Interactable->MeshComp)
        return false;

    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + Camera->GetForwardVector() * 1000.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    if (!bHit)
        return false;

    // comprobar que el HIT ES EL STATIC MESH
    return Hit.GetComponent() == Interactable->MeshComp;
}

bool AAzulCharacterBase::HasInteractable() const
{
    return CurrentInteractable != nullptr;
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
    UE_LOG(LogTemp, Error, TEXT("CHARACTER: ESPACIO PRESIONADO"));

    /*if (HiloComponent)
    {
        HiloComponent->ToggleHilo();
    }*/

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
