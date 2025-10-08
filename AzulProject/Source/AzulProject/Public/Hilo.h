#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void SetSpline();

};
