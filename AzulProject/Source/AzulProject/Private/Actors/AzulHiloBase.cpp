#include "Actors/AzulHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Kismet/GameplayStatics.h"

AAzulHiloBase::AAzulHiloBase()
{
    bHiloVisible = false;

    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
    RootComponent = SplineComp;

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
    NiagaraComp->SetupAttachment(SplineComp);

    NiagaraComp->SetVisibility(false, true);
    NiagaraComp->SetAutoActivate(false);
}


void AAzulHiloBase::BeginPlay()
{
    Super::BeginPlay();

    CachedPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (NiagaraTemplate)
    {
        NiagaraComp->SetAsset(NiagaraTemplate);
    }
}


void AAzulHiloBase::SetNiagaraLifeTime(float Value)
{
    static const FName LifeTimeName(TEXT("LifeTime"));
    NiagaraComp->SetVariableFloat(LifeTimeName, Value);

    //UKismetSystemLibrary::PrintString(this, TEXT("CAMBIADO EL LIFE TIME"), true);
}



TArray<FVector> AAzulHiloBase::GenerateCurvedRoute(
    const FVector& StartPos,
    const FVector& StartTangentDir,
    const FVector& EndPos
)
{
    TArray<FVector> Result;

    const int32 NumPoints = 20;

    // Control point alineado con el tramo recto
    FVector ControlPoint = StartPos + StartTangentDir * 60.0f;

    for (int32 i = 1; i <= NumPoints; ++i)
    {
        const float Alpha = static_cast<float>(i) / NumPoints;

        FVector Point =
            FMath::Pow(1.0f - Alpha, 2) * StartPos +
            2.0f * (1.0f - Alpha) * Alpha * ControlPoint +
            FMath::Pow(Alpha, 2) * EndPos;

        Result.Add(Point);
    }

    return Result;
}



void AAzulHiloBase::ApplyInterpolatedSplinePoints(const TArray<FVector>& Points)
{
    //UKismetSystemLibrary::PrintString(
    //    this,
    //    TEXT(">>> ClearSplinePoints CALLED <<<"),
    //    true
    //);

    SplineComp->ClearSplinePoints(true);

    for (int32 i = 0; i < Points.Num(); i++)
    {
        SplineComp->AddSplinePoint(Points[i], ESplineCoordinateSpace::World);

    }

    // Para evitar overshoot DEFINITIVAMENTE
    for (int32 i = 0; i < Points.Num(); i++)
    {
       SplineComp->SetSplinePointType(i, ESplinePointType::CurveClamped, false);
    }

    // Actualizar spline
    SplineComp->SetClosedLoop(false);
    SplineComp->UpdateSpline();

    //UKismetSystemLibrary::PrintString(
    //    this,
    //    FString::Printf(
    //        TEXT("SplinePoints: %d | Length: %.2f"),
    //        SplineComp->GetNumberOfSplinePoints(),
    //        SplineComp->GetSplineLength()
    //    ),
    //    true
    //);


    // VFX
    static const FName SpawnRateName(TEXT("SpawnRate"));
    NiagaraComp->SetFloatParameter(SpawnRateName, SplineComp->GetSplineLength());

    NiagaraComp->SetVisibility(true, true);
    NiagaraComp->ReinitializeSystem();
    NiagaraComp->ResetSystem();
    NiagaraComp->Activate(true);
}

void AAzulHiloBase::RecalculateHiloFromInput()
{
    //UKismetSystemLibrary::PrintString(
    //    this,
    //    TEXT(">>> RecalculateHiloFromInput CALLED <<<"),
    //    true
    //);

    if (!CachedPlayer || !HijoActor)
    {
        //UKismetSystemLibrary::PrintString(
        //    this,
        //    TEXT("EARLY RETURN: CachedPlayer or HijoActor NULL"),
        //    true
        //);
        //return;
    }

    SplineComp->SetVisibility(true, true);

    //UKismetSystemLibrary::PrintString(
    //    this,
    //    FString::Printf(
    //        TEXT("bHiloVisible cambiado: %s"),
    //        bHiloVisible ? TEXT("true") : TEXT("false")
    //    ),
    //    true
    //);

    const float HiloZ = CachedPlayer->GetActorLocation().Z - 20.0f;

    FVector StartPos = CachedPlayer->GetActorLocation();
    StartPos.Z = HiloZ;

    const FVector ForwardDir = CachedPlayer->GetActorForwardVector();

    FVector SecondPoint = StartPos + ForwardDir * 50.0f;
    SecondPoint.Z = HiloZ;

    FVector EndPos = HijoActor->GetActorLocation();

    PreviousPoints = TargetPoints;
    TargetPoints.Empty();

    TargetPoints.Add(StartPos);
    TargetPoints.Add(SecondPoint);

    const TArray<FVector> CurvedPoints =
        GenerateCurvedRoute(SecondPoint, ForwardDir, EndPos);

    TargetPoints.Append(CurvedPoints);

    OnSplineRouteChanged.Broadcast(PreviousPoints, TargetPoints);

    ApplyInterpolatedSplinePoints(TargetPoints);

    bHiloVisible = true;

    SetNiagaraLifeTime(100.0f);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(Timer_HideHilo);
        GetWorld()->GetTimerManager().SetTimer(
            Timer_HideHilo,
            this,
            &AAzulHiloBase::HideHilo,
            HiloVisibleTime,
            false
        );
    }
}





void AAzulHiloBase::HideHilo()
{
    GetWorld()->GetTimerManager().ClearTimer(Timer_HideHilo);

    // ❗ NO borrar spline
    SplineComp->SetVisibility(false, true);

    NiagaraComp->Deactivate();
    NiagaraComp->SetVisibility(false, true);

    bHiloVisible = false;
}
