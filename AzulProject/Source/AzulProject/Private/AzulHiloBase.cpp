#include "AzulHiloBase.h"
#include "EngineUtils.h" // Necesario para TActorIterator
#include "AzulCharacterBase.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"

AAzulHiloBase::AAzulHiloBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	RootComponent = SplineComp;
	SplineComp->SetMobility(EComponentMobility::Movable);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(SplineComp);

	// Cargar el Niagara System una sola vez en tiempo de construcción
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("/Game/VFX/NS_Hilo.NS_Hilo"));

	if (NiagaraAsset.Succeeded())
	{
		NiagaraTemplate = NiagaraAsset.Object;
		NiagaraComp->SetAsset(NiagaraTemplate);
	}
}

void AAzulHiloBase::BeginPlay()
{
	Super::BeginPlay();

	// Si hay plantilla, asignarla al componente y activarlo.
	if (NiagaraComp)
	{
		if (NiagaraTemplate)
		{
			NiagaraComp->SetAsset(NiagaraTemplate);
			NiagaraComp->Activate(true);
		}
	}

	AAzulCharacterBase* Other = GetSonCharacter(GetWorld());
	if (Other)
	{
		EndPoint = Other->GetActorLocation();
		// Usar EndPoint para definir el punto final del spline
	}

}

// Called every frame
void AAzulHiloBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAzulHiloBase::UpdateSpline_Implementation(const FVector& NewStartPosition)
{
	//Cambiar el SpawnRate del Hilo con el Length del Spline
	
	SplineComp->ClearSplinePoints(false);

	// Cambiar punto inicial
	SplineComp->AddSplinePoint(FVector(300,200,0), ESplineCoordinateSpace::Local);
	SplineComp->AddSplinePoint(FVector(400, 600, 0), ESplineCoordinateSpace::Local);


	// Asegurar coherencia
	SplineComp->UpdateSpline();
}

AAzulCharacterBase* AAzulHiloBase::GetSonCharacter(UWorld* World)
{
	if (!World) return nullptr;

	APawn* Mom = World->GetFirstPlayerController()->GetPawn();
	if (!Mom) return nullptr;

	for (TActorIterator<AAzulCharacterBase> It(World); It; ++It)
	{
		AAzulCharacterBase* Son = *It;
		if (Son && Son != Mom)
		{
			return Son; // Solo hay dos, devolvemos el hijo
		}
	}

	return nullptr; // No encontrado
}

