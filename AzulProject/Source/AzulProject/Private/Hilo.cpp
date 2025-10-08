// Fill out your copyright notice in the Description page of Project Settings.


#include "Hilo.h"

// Sets default values
AHilo::AHilo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void AHilo::SetSpline()
{
	Spline->ClearSplinePoints(false);

	Spline->AddSplinePoint(FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(0, 0, 300), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(300, 0, 0), ESplineCoordinateSpace::Local);
	Spline->AddSplinePoint(FVector(300, 0, 300), ESplineCoordinateSpace::Local);

	Spline->UpdateSpline();

}

