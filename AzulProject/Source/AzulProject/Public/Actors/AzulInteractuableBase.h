#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "AzulInteractuableBase.generated.h"

class AAzulCharacterBase;

UCLASS()
class AZULPROJECT_API AAzulInteractuableBase : public AActor, public IAzulInteractuableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzulInteractuableBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Componentes")
	USceneComponent* RootComp;

	// Visual representation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Azul|Interactuable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	// Componente de colisión que genera overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Interactuable", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;
	//-----------------------------------SOLO PARA EL BEBE O LA MADRE CUANDO LLEGUE SU MOMENTO ----------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Hilo")
	USceneComponent* HiloEndPoint;


	//

	UFUNCTION(BlueprintCallable, Category = "Azul|Interactuable")
	bool IsValidInteractionComponent(const UPrimitiveComponent* Component) const;

	// Componentes válidos para interacción por mirilla
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Azul|Interactuable")
	TArray<UPrimitiveComponent*> InteractionComponents;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Character")
	AAzulCharacterBase* OverlappingCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Tags")
	FGameplayTag InteractTag;

	// Función llamada cuando ocurre overlap (debe ser UFUNCTION para AddDynamic)
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// Función llamada cuando termina el overlap (debe ser UFUNCTION para AddDynamic)
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Azul|Interactuable")
	void OnExtraInteractBP();

	virtual void Interactua_Implementation() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
