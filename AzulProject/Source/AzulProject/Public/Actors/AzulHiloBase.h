// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Interfaces/AzulHiloInterface.h"
#include "Characters/AzulCharacterBase.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "AzulHiloBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulHiloBase : public AActor, public IAzulHiloInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzulHiloBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spline
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hilo")
	USplineComponent* SplineComp;

	UPROPERTY(EditAnywhere, Category = "Hilo")
	bool bLoopSpline = false;

	UPROPERTY(EditAnywhere, Category = "Hilo")
	FVector EndPoint;

	// Niagara
	UPROPERTY(EditAnywhere, Category = "Niagara")
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* NiagaraComp;

	//INTERFAZ
	virtual void UpdateSpline_Implementation(const FVector& NewStartPosition) override;

private:
	AAzulCharacterBase* GetSonCharacter(UWorld* World);
};
