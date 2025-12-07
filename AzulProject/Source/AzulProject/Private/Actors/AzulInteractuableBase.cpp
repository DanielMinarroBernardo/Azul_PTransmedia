#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"




// Sets default values
AAzulInteractuableBase::AAzulInteractuableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Crear el componente raíz (puede ser cualquier USceneComponent)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	

	// Sphere collision (trigger)
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->InitSphereRadius(100.0f); // valor por defecto editable en BP si se expone
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetGenerateOverlapEvents(true);

	// Static mesh (visual)
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	// Dejar la colisión del mesh desactivada: la esfera manejará los overlaps
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetGenerateOverlapEvents(false);

	// Widget component
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->SetupAttachment(RootComponent);
	WidgetComp->SetWidgetSpace(EWidgetSpace::World); // o EWidgetSpace::Screen según prefieras
	WidgetComp->SetDrawSize(FVector2D(1280.0, 720.0));
	WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetGenerateOverlapEvents(false);
	WidgetComp->SetVisibility(false); // oculto por defecto

}

// Called when the game starts or when spawned
void AAzulInteractuableBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAzulInteractuableBase::OnBeginOverlap);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AAzulInteractuableBase::OnEndOverlap);
	}

}

void AAzulInteractuableBase::Interactua_Implementation()
{
	OnExtraInteractBP();
}

// Called every frame
void AAzulInteractuableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AAzulInteractuableBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificar que el actor que entra sea del tipo AzulCharacterBase o derivado
	OverlappingCharacter = Cast<AAzulCharacterBase>(OtherActor);
	if (!OverlappingCharacter)
	{
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}

	UKismetSystemLibrary::PrintString(
		this,
		TEXT("overlapando"),
		true,        // Mostrar en pantalla
		false,       // No imprimir en log
		FLinearColor::Green,
		2.0f         // Duración en segundos
	);


	// Mostrar widget si existe
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(true);
	}

	OverlappingCharacter->CurrentInteractableClass = GetClass();

	//Asignar a sí mismo como el interactuable del Character cuando overlapa
	OverlappingCharacter->CurrentInteractable = TScriptInterface<IAzulInteractuableInterface>(this);
}

void AAzulInteractuableBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Verificar que el actor que sale sea del tipo AzulCharacterBase o derivado
	OverlappingCharacter = Cast<AAzulCharacterBase>(OtherActor);
	if (!OverlappingCharacter)
	{
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}
	// Ocultar widget si existe
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(false);
	}

	OverlappingCharacter->CurrentInteractableClass = nullptr;

	// Limpiar la referencia al interactuable en el Character cuando deja de overlapar
	OverlappingCharacter->CurrentInteractable = nullptr;
}




