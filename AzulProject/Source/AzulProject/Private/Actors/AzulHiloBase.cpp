#include "Actors/AzulHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
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

    if (NiagaraTemplate)
    {
        NiagaraComp->SetAsset(NiagaraTemplate);
        NiagaraComp->Activate(true);
    }
}

//
// INTERFAZ — solo se llama cuando un trigger la activa
//
void AAzulHiloBase::UpdateSpline_Implementation(const FVector& TriggerPos)
{
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player || !HijoActor) return;

    FVector Start = Player->GetActorLocation();
    Start.Z = 0.f;

    FVector End = HijoActor->GetActorLocation();
    End.Z = 0.f;

    TArray<FVector> NewPoints = GenerateCurvedRoute(Start, End);

    if (PreviousPoints.Num() == 0)
        PreviousPoints = NewPoints;

    OnSplineRouteChanged.Broadcast(PreviousPoints, NewPoints);

    PreviousPoints = NewPoints;
}



TArray<FVector> AAzulHiloBase::GenerateCurvedRoute(const FVector& StartPos, const FVector& EndPos)
{
    TArray<FVector> Points;

    FVector A(StartPos.X, StartPos.Y, 0.f);
    FVector B(EndPos.X, EndPos.Y, 0.f);

    FVector Dir = (B - A).GetSafeNormal();
    FVector Perp(Dir.Y, -Dir.X, 0.f);   // perpendicular estable en 2D

    float Dist = FVector::Distance(A, B);

    // offset muy pequeño -> NUNCA puede crear un círculo
    float CurveStrength = FMath::Clamp(Dist * 0.15f, 20.f, 80.f);

    FVector Mid = (A + B) * 0.5f + (Perp * CurveStrength);
    Mid.Z = 0.f;

    // 3 puntos = curva ligera
    Points.Add(A);
    Points.Add(Mid);
    Points.Add(B);

    return Points;
}




void AAzulHiloBase::ApplyInterpolatedSplinePoints(const TArray<FVector>& Points)
{
    SplineComp->ClearSplinePoints(true);

    for (int32 i = 0; i < Points.Num(); i++)
    {
        // Los puntos vienen ya en WORLD → usar WORLD
        FVector Flat = FVector(Points[i].X, Points[i].Y, 0.f);

        SplineComp->AddSplinePoint(Flat, ESplineCoordinateSpace::World);
    }

    // Para evitar overshoot DEFINITIVAMENTE
    for (int32 i = 0; i < Points.Num(); i++)
    {
        SplineComp->SetSplinePointType(i, ESplinePointType::Linear, false);
        // Si quieres ligera curva:
        // SplineComp->SetSplinePointType(i, ESplinePointType::CurveClamped, false);
    }

    // Actualizar spline
    SplineComp->SetClosedLoop(false);
    SplineComp->UpdateSpline();

    // VFX
    NiagaraComp->SetFloatParameter("SpawnRate", SplineComp->GetSplineLength());
    NiagaraComp->ResetSystem();
}



