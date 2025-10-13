// Fill out your copyright notice in the Description page of Project Settings.


#include "Hilo.h"

// Sets default values
AHilo::AHilo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- Trigger de colisión (Root) ---
	//TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//SetRootComponent(TriggerBox);
	//TriggerBox->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	//TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	//TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	//TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//TriggerBox->SetGenerateOverlapEvents(true);

	// Bind de evento de overlap
	//TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHilo::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void AHilo::BeginPlay()
{
	Super::BeginPlay();

	Spline = Cast<USplineComponent>(GetDefaultSubobjectByName(TEXT("Recorrido")));

	if (!Spline)
	{
		UE_LOG(LogTemp, Error, TEXT("No se encontró el Spline dentro del actor"));
		return;
	}
	
}

// Called every frame
void AHilo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHilo::SetShapeSpline()
{
	Spline->ClearSplinePoints(false);

	Spline->AddSplinePoint(FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(0, 0, 300), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(300, 0, 0), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(300, 0, 300), ESplineCoordinateSpace::Local);

	Spline->UpdateSpline();

}

void AHilo::SetLengthHilo()
{
	NiagaraHilo = Cast<UNiagaraComponent>(GetDefaultSubobjectByName(TEXT("HiloNiagara")));

	const float Length = Spline->GetSplineLength();

	NiagaraHilo->SetFloatParameter(FName(TEXT("SpawnRate")), Length);
	NiagaraHilo->SetFloatParameter(FName(TEXT("User.SpawnRate")), Length);
}

//void AHilo::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// Evitar auto-disparo y filtrar a Pawns (jugador/NPCs)
//	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Overlap con %s -> SetSpline()"), *OtherActor->GetName());
//		SetSpline();
//	}
//}

