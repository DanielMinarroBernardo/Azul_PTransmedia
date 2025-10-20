
#include "AzulTriggerHiloBase.h"
#include "AzulCharacterBase.h"
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

	// Cargar la clase Blueprint del hilo
	static ConstructorHelpers::FClassFinder<AAzulHiloBase> HiloBP(TEXT("/Game/Code/BP_Hilo"));
	if (HiloBP.Succeeded())
	{
		HiloActorClass = HiloBP.Class;
	}

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

	// Buscar el actor del hilo si no se asignó manualmente
	if (!HiloActor && HiloActorClass)
	{
		for (TActorIterator<AAzulHiloBase> It(GetWorld(), HiloActorClass); It; ++It)
		{
			HiloActor = *It;
			break; // Usa el primero encontrado
		}
	}
}

// Called every frame
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
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "No");
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}

	// Verificar que haya un hilo asignado y que implemente la interfaz
	if (HiloActor && HiloActor->GetClass()->ImplementsInterface(UAzulHiloInterface::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "si");
		// Llamar a la función UpdateSpline definida en la interfaz
		IAzulHiloInterface::Execute_UpdateSpline(HiloActor, GetActorLocation());
	}
}

