#include "Actors/AzulAscensorBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

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
    SetActorLocation(Current + Direction * Step);
}

void AAzulAscensorBase::Interactua_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("INTERACTUA ASCENSOR"));

    if (bIsMoving)
        return;

    // Alinear personaje con el ascensor si está encima
    if (OverlappingCharacter)
    {
        FVector CharLocation = OverlappingCharacter->GetActorLocation();
        CharLocation.Z = GetActorLocation().Z + 10.0f;
        OverlappingCharacter->SetActorLocation(CharLocation);
    }

    bIsMoving = true;

    GetWorld()->GetTimerManager().SetTimer(
        MoveHandle,
        this,
        &AAzulAscensorBase::MoveTick,
        0.01f,
        true
    );

    // Mantener al personaje “pegado” al ascensor
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
