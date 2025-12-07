#include "Actors/AzulHiloBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AAzulHiloBase::AAzulHiloBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	RootComponent = SplineComp;
}

void AAzulHiloBase::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayer = Cast<AAzulCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// --------------------------------------------------------------------------------------
// GENERAR CURVA ENTRE PLAYER Y HIJOACTOR
// --------------------------------------------------------------------------------------

TArray<FVector> AAzulHiloBase::GenerateCurvedRoute(const FVector& StartPos, const FVector& EndPos)
{
	TArray<FVector> FinalPoints;

	FVector A(StartPos.X, StartPos.Y, StartPos.Z);
	FVector B(EndPos.X, EndPos.Y, EndPos.Z);

	FVector AB = B - A;
	float Dist = AB.Size();

	if (Dist < KINDA_SMALL_NUMBER)
	{
		FinalPoints.Add(A);
		FinalPoints.Add(B);
		return FinalPoints;
	}

	FVector Dir = AB.GetSafeNormal();
	FVector Perp(Dir.Y, -Dir.X, 0.f);

	float CurveStrength = FMath::Clamp(Dist * 0.15f, 30.f, 150.f);

	FVector C1 = A + Dir * (Dist * 0.25f) + Perp * CurveStrength;
	FVector C2 = A + Dir * (Dist * 0.75f) + Perp * CurveStrength;

	const int32 Subdivisions = 32;

	for (int32 i = 0; i <= Subdivisions; i++)
	{
		float t = (float)i / Subdivisions;

		FVector P =
			FMath::Pow(1 - t, 3) * A +
			3 * FMath::Pow(1 - t, 2) * t * C1 +
			3 * (1 - t) * FMath::Pow(t, 2) * C2 +
			FMath::Pow(t, 3) * B;

		FinalPoints.Add(P);
	}

	return FinalPoints;
}

// --------------------------------------------------------------------------------------
// LLAMADO DESDE EL TRIGGER VIA INTERFAZ
// --------------------------------------------------------------------------------------

void AAzulHiloBase::UpdateSpline_Implementation(const FVector& TriggerPos)
{
	if (!HijoActor) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	FVector Start = Player->GetActorLocation();
	FVector End = HijoActor->GetActorLocation();

	// Generar curva
	TArray<FVector> Points = GenerateCurvedRoute(Start, End);

	// Actualizar spline
	ApplySplinePoints(Points);
}

// --------------------------------------------------------------------------------------
// ACTUALIZAR EL SPLINE CON LOS NUEVOS PUNTOS
// --------------------------------------------------------------------------------------

void AAzulHiloBase::ApplySplinePoints(const TArray<FVector>& Points)
{
	SplineComp->ClearSplinePoints(true);

	for (const FVector& P : Points)
	{
		SplineComp->AddSplinePoint(P, ESplineCoordinateSpace::World);
	}

	SplineComp->SetClosedLoop(false);
	SplineComp->UpdateSpline();

	UpdateSplineMeshes();
}

// --------------------------------------------------------------------------------------
// ACTUALIZAR LOS SEGMENTOS DE TUBERÍA (SPLINE MESH COMPONENTS)
// --------------------------------------------------------------------------------------

void AAzulHiloBase::UpdateSplineMeshes()
{
	int32 NumPoints = SplineComp->GetNumberOfSplinePoints();
	if (!TuboMesh || NumPoints < 2)
		return;

	// Crear los segmentos necesarios
	while (SplineMeshes.Num() < NumPoints - 1)
	{
		USplineMeshComponent* NewSeg = NewObject<USplineMeshComponent>(this);

		NewSeg->SetStaticMesh(TuboMesh);
		if (TuboMaterial)
			NewSeg->SetMaterial(0, TuboMaterial);

		// CRUCIAL: permitir que se mueva en runtime
		NewSeg->SetMobility(EComponentMobility::Movable);

		// Asegurar que la tubería queda tumbada
		NewSeg->SetForwardAxis(ESplineMeshAxis::X);
		NewSeg->SetStartRoll(90.f);
		NewSeg->SetEndRoll(90.f);

		NewSeg->RegisterComponent();
		NewSeg->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		SplineMeshes.Add(NewSeg);
	}

	// Eliminar sobras
	while (SplineMeshes.Num() > NumPoints - 1)
	{
		SplineMeshes.Pop()->DestroyComponent();
	}

	// Actualizar cada segmento con posición + tangente correctas
	for (int32 i = 0; i < NumPoints - 1; i++)
	{
		FVector StartPos = SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndPos = SplineComp->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

		FVector StartTan = SplineComp->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndTan = SplineComp->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

		SplineMeshes[i]->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan);
	}
}
