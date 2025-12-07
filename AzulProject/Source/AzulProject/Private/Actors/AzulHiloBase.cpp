#include "Actors/AzulHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Actors/AzulTriggerHiloBase.h"
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

    if (NiagaraTemplate)
    {
        NiagaraComp->SetAsset(NiagaraTemplate);
        NiagaraComp->Activate(true);
    }
}

void AAzulHiloBase::SetNiagaraLifeTime()
{
    NiagaraComp->SetNiagaraVariableFloat("LifeTime", 2.0f);
    UKismetSystemLibrary::PrintString(this, "CAMBIADO EL LIFE TIME", true);
}

TArray<FVector> AAzulHiloBase::GenerateCurvedRoute(const FVector& StartPos, const FVector& EndPos)
{
    TArray<FVector> FinalPoints;

    // --------------------------
    // 1) Preparamos A y B
    // --------------------------
    FVector A(StartPos.X, StartPos.Y, SueloZ);
    FVector B(EndPos.X, EndPos.Y, SueloZ);

    FVector AB = (B - A);
    float Dist = AB.Size();

    if (Dist < KINDA_SMALL_NUMBER)
    {
        FinalPoints.Add(A);
        FinalPoints.Add(B);
        return FinalPoints;
    }

    FVector Dir = AB.GetSafeNormal();
    FVector Perp(Dir.Y, -Dir.X, 0.f); // perpendicular estable 2D

    // Curvatura ligera
    float CurveStrength = FMath::Clamp(Dist * 0.18f, 30.f, 120.f);

    // --------------------------
    // 2) Puntos de control Bezier
    // --------------------------
    FVector C1 = A + Dir * (Dist * 0.25f) + Perp * CurveStrength;
    FVector C2 = A + Dir * (Dist * 0.75f) + Perp * CurveStrength;

    C1.Z = SueloZ;
    C2.Z = SueloZ;

    // --------------------------
    // 3) Subdividir Bezier en 32 pasos
    // --------------------------
    const int32 Subdivisions = 32;

    for (int32 i = 0; i <= Subdivisions; i++)
    {
        float t = (float)i / (float)Subdivisions;

        FVector P =
            FMath::Pow(1 - t, 3) * A +
            3 * FMath::Pow(1 - t, 2) * t * C1 +
            3 * (1 - t) * FMath::Pow(t, 2) * C2 +
            FMath::Pow(t, 3) * B;

        FinalPoints.Add(P);
    }

    return FinalPoints;
}


void AAzulHiloBase::UpdateSpline_Implementation(const UE::Math::TVector<double>& Posicion)
{
    NiagaraComp->SetNiagaraVariableFloat("LifeTime", 500.0f);

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


void AAzulHiloBase::ApplyInterpolatedSplinePoints(const TArray<FVector>& Points)
{
    SplineComp->ClearSplinePoints(true);

    for (int32 i = 0; i < Points.Num(); i++)
    {
        // Los puntos vienen ya en WORLD → usar WORLD
        FVector Flat = FVector(Points[i].X, Points[i].Y, SueloZ);

        SplineComp->AddSplinePoint(Flat, ESplineCoordinateSpace::World);
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



