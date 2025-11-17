#include "Actors/AzulTriggerHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Components/SphereComponent.h"
#include "Interfaces/AzulHiloInterface.h"
#include "Characters/AzulCharacterBase.h"
#include "EngineUtils.h" // Para TActorIterator

// Sets default values
AAzulTriggerHiloBase::AAzulTriggerHiloBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creación del componente de colisión esférica
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	SetRootComponent(TriggerSphere);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	TriggerSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAzulTriggerHiloBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Bindear correctamente al método de overlap
	if (TriggerSphere)
	{
		TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AAzulTriggerHiloBase::OnBeginOverlap);
	}
}
void AAzulTriggerHiloBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAzulTriggerHiloBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificar que el actor que entra sea del tipo AzulCharacterBase o derivado
	AAzulCharacterBase* OverlappingCharacter = Cast<AAzulCharacterBase>(OtherActor);
	if (!OverlappingCharacter)
	{
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}

	// Verificar que haya un hilo asignado y que implemente la interfaz
	if (HiloActor && HiloActor->GetClass()->ImplementsInterface(UAzulHiloInterface::StaticClass()))
	{
		FVector Pos = GetActorLocation();
		Pos.Z = 0.f;

		// Llamar a la función UpdateSpline definida en la interfaz
		IAzulHiloInterface::Execute_UpdateSpline(HiloActor, Pos);
	}
}

