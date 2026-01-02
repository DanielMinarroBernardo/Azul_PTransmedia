#include "Actors/AzulHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Kismet/GameplayStatics.h"

AAzulHiloBase::AAzulHiloBase()
{
    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
    RootComponent = SplineComp;

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
    NiagaraComp->SetupAttachment(SplineComp);
}

void AAzulHiloBase::BeginPlay()
{
    Super::BeginPlay();

    CachedPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (NiagaraTemplate)
    {
        NiagaraComp->SetAsset(NiagaraTemplate);
        NiagaraComp->Activate(true);
    }

    //SplineComp->SetVisibility(false, true);
    //NiagaraComp->SetVisibility(false, true);
}

void AAzulHiloBase::SetNiagaraLifeTime()
{
    NiagaraComp->SetNiagaraVariableFloat("LifeTime", 2.0f);
    UKismetSystemLibrary::PrintString(this, "CAMBIADO EL LIFE TIME", true);
}

TArray<FVector> AAzulHiloBase::GenerateCurvedRoute(
    const FVector& StartPos,
    const FVector& StartTangentDir,
    const FVector& EndPos
)
{
    TArray<FVector> Result;

    const int32 NumPoints = 12;

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

    // VFX
    NiagaraComp->SetFloatParameter("SpawnRate", SplineComp->GetSplineLength());
    NiagaraComp->ResetSystem();
}

void AAzulHiloBase::RecalculateHiloFromInput()
{
    if (!CachedPlayer || !HijoActor)
    {
        return;
    }

    // Altura solo para el inicio del hilo
    const float HiloZ = CachedPlayer->GetActorLocation().Z - 20.0f;

    // =========================
    // START
    // =========================
    FVector StartPos = CachedPlayer->GetActorLocation();
    StartPos.Z = HiloZ;

    // =========================
    // TRAMO RECTO INICIAL
    // =========================
    const FVector ForwardDir = CachedPlayer->GetActorForwardVector();

    FVector SecondPoint = StartPos + ForwardDir * 50.0f;
    SecondPoint.Z = HiloZ;

    // =========================
    // END (usar Z real del hijo)
    // =========================
    FVector EndPos = HijoActor->GetActorLocation();

    // =========================
    // ESTADO ANTERIOR
    // =========================
    PreviousPoints = TargetPoints;
    TargetPoints.Empty();

    // =========================
    // CONSTRUCCIÓN DE RUTA
    // =========================

    // Tramo recto
    TargetPoints.Add(StartPos);
    TargetPoints.Add(SecondPoint);

    // Curva continua heredando la tangente del tramo recto
    const TArray<FVector> CurvedPoints =
        GenerateCurvedRoute(
            SecondPoint,   // Inicio real de la curva
            ForwardDir,    // Tangente inicial (continuidad)
            EndPos         // Destino final
        );

    TargetPoints.Append(CurvedPoints);

    // =========================
    // NOTIFICAR CAMBIO DE SPLINE
    // =========================
    OnSplineRouteChanged.Broadcast(PreviousPoints, TargetPoints);

    // =========================
    // VISIBILIDAD
    // =========================
    bHiloVisible = true;

    if (NiagaraComp)
    {
        NiagaraComp->SetVisibility(true);
        NiagaraComp->Activate(true);
    }

    // =========================
    // TIMER DE OCULTADO
    // =========================
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
