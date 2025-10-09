#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"  
#include "Hilo.generated.h"

UCLASS()
class AZULPROJECT_API AHilo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHilo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* Spline;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	//UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* NiagaraHilo;

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetShapeSpline();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetLengthHilo();



//private:
//	/** Handler del begin overlap del TriggerBox */
//	UFUNCTION()
//	void OnBoxBeginOverlap(
//		UPrimitiveComponent* OverlappedComp,
//		AActor* OtherActor,
//		UPrimitiveComponent* OtherComp,
//		int32 OtherBodyIndex,
//		bool bFromSweep,
//		const FHitResult& SweepResult
//	);
};
