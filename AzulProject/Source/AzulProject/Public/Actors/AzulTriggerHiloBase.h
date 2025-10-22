// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AzulHiloBase.h"
#include "Characters/AzulCharacterBase.h"
#include "Interfaces/AzulHiloInterface.h"
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

	// Referencia al actor del hilo (AzulHiloBase o su Blueprint)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Hilo", meta = (DisplayName = "Actor del Hilo", ToolTip = "Actor del que se leer� la Location"))
	AAzulHiloBase* HiloActor = nullptr;


	// Evento de solapamiento
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
