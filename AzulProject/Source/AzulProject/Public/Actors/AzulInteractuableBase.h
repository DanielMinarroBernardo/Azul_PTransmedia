#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/AzulCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "AzulInteractuableBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulInteractuableBase : public AActor, public IAzulInteractuableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzulInteractuableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Visual representation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactuable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	// Componente de colisión que genera overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactuable", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	// Widget component para mostrar UI (ej.: prompt de interacción)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComp;

	// Función llamada cuando ocurre overlap (debe ser UFUNCTION para AddDynamic)
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// Función llamada cuando termina el overlap (debe ser UFUNCTION para AddDynamic)
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
