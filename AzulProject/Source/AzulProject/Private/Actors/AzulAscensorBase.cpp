#include "Actors/AzulAscensorBase.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"


AAzulAscensorBase::AAzulAscensorBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAzulAscensorBase::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();

    TargetLocationUp = StartLocation;
    TargetLocationDown = FVector(StartLocation.X, StartLocation.Y, 359.f);
}

void AAzulAscensorBase::MoveTick()
{
    FVector Current = GetActorLocation();
    FVector Target = bIsUp ? TargetLocationDown : TargetLocationUp;

    float Step = Speed * 0.01f;
    float Distance = FVector::Dist(Current, Target);

    if (Distance <= Step)
    {
        SetActorLocation(Target);
        GetWorld()->GetTimerManager().ClearTimer(MoveHandle);
        bIsMoving = false;


        UE_LOG(LogTemp, Warning, TEXT("ASCENSOR: FIN DEL MOVIMIENTO"));
        return;
    }

    FVector Direction = (Target - Current).GetSafeNormal();
    FVector NewPos = Current + Direction * Step;

    SetActorLocation(NewPos);

}

void AAzulAscensorBase::Interactua_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("INTERACTUA ASCENSOR"));

    if (bIsMoving)
        return;

    if (OverlappingCharacter)
    {
        FVector CharLocation = OverlappingCharacter->GetActorLocation();
        CharLocation.Z = GetActorLocation().Z + 10.0f;     // subir 10 unidad exacta
        OverlappingCharacter->SetActorLocation(CharLocation);
    }

    bIsMoving = true;

    ActivateIMCLook();

    GetWorld()->GetTimerManager().SetTimer(
        MoveHandle,
        this,
        &AAzulAscensorBase::MoveTick,
        0.01f,
        true
    );

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    AAzulCharacterBase* Character = Cast<AAzulCharacterBase>(PC->GetPawn());

    FTimerHandle DelayHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DelayHandle,
        [Character]() {
            Character->DisableMappingContext(TEXT("IMC_Look"));
            Character->EnableMappingContext(TEXT("IMC_Default"), 1);
        },
        2.0f,
        false
    );

    FTimerHandle MoveCharacterHandle;

    GetWorld()->GetTimerManager().SetTimer(
        MoveCharacterHandle,
        [this]()
        {
            if (OverlappingCharacter)
            {
                FVector CharLoc = OverlappingCharacter->GetActorLocation();
                CharLoc.Z = GetActorLocation().Z + 100.0f;
                OverlappingCharacter->SetActorLocation(CharLoc);
            }
        },
        0.01f,
        true
    );


    bIsUp = !bIsUp;
}

void AAzulAscensorBase::ActivateIMCLook()
{
    if (!OverlappingCharacter) return;

    OverlappingCharacter->DisableMappingContext(TEXT("IMC_Default"));
    OverlappingCharacter->EnableMappingContext(TEXT("IMC_Look"), 2);
}

void AAzulAscensorBase::ActivateIMCDefault()
{
    if (!OverlappingCharacter) return;

    OverlappingCharacter->DisableMappingContext(TEXT("IMC_Look"));
    OverlappingCharacter->EnableMappingContext(TEXT("IMC_Default"), 1);
}

