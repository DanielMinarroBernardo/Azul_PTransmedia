#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "AzulHiloInterface.h"
#include "AzulCharacterBase.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "AzulHiloBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulHiloBase : public AActor, public IAzulHiloInterface
{
	GENERATED_BODY()

public:
	AAzulHiloBase();

protected:
	virtual void BeginPlay() override;

public:
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
