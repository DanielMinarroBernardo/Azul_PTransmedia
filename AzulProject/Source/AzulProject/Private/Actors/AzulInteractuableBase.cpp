#include "Actors/AzulInteractuableBase.h"

// Sets default values
AAzulInteractuableBase::AAzulInteractuableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static mesh (visual)
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// Usar el mesh como root para la facilidad de edici�n en el editor
	SetRootComponent(MeshComp);
	// Dejar la colisi�n del mesh desactivada: la esfera manejar� los overlaps
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetGenerateOverlapEvents(false);

	// Sphere collision (trigger)
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->InitSphereRadius(100.0f); // valor por defecto editable en BP si se expone
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetGenerateOverlapEvents(true);

	// Widget component
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->SetupAttachment(RootComponent);
	WidgetComp->SetWidgetSpace(EWidgetSpace::World); // o EWidgetSpace::Screen seg�n prefieras
	WidgetComp->SetDrawSize(FVector2D(1280.0, 720.0));
	WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
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

// Called every frame
void AAzulInteractuableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAzulInteractuableBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificar que el actor que entra sea del tipo AzulCharacterBase o derivado
	AAzulCharacterBase* OverlappingCharacter = Cast<AAzulCharacterBase>(OtherActor);
	if (!OverlappingCharacter)
	{
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}

	// Mostrar widget si existe
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(true);
	}

}

void AAzulInteractuableBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Verificar que el actor que sale sea del tipo AzulCharacterBase o derivado
	AAzulCharacterBase* OverlappingCharacter = Cast<AAzulCharacterBase>(OtherActor);
	if (!OverlappingCharacter)
	{
		return; // Si no es un personaje AzulCharacterBase, no hacer nada
	}
	// Ocultar widget si existe
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(false);
	}
}