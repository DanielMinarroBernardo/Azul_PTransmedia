// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AzulHiloBase.h"
#include "AzulCharacterBase.h"
#include "AzulHiloInterface.h"
#include "AzulTriggerHiloBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulTriggerHiloBase : public AActor, public IAzulHiloInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzulTriggerHiloBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Clase del actor de hilo (Blueprint derivado de AzulHiloBase)
	UPROPERTY(EditDefaultsOnly, Category = "Hilo")
	TSubclassOf<AAzulHiloBase> HiloActorClass;

	// Referencia al actor del hilo (AzulHiloBase o su Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hilo")
	AAzulHiloBase* HiloActor;

	// Evento de solapamiento
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
