// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AzulHiloBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Interfaces/AzulHiloInterface.h"
#include "AzulTriggerHiloBase.generated.h"

class AAzulCharacterBase;

UCLASS()
class AZULPROJECT_API AAzulTriggerHiloBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzulTriggerHiloBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Azul|Hilo")
	USphereComponent* TriggerSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Referencia al actor del hilo (AzulHiloBase o su Blueprint)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Azul|Hilo")
	AAzulHiloBase* HiloActor = nullptr;


	// Evento de solapamiento
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
