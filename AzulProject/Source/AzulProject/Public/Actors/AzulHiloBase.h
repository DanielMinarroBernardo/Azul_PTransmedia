#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Interfaces/AzulHiloInterface.h"
#include "AzulHiloBase.generated.h"

class AAzulInteractuableBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSplineRouteChanged,
	const TArray<FVector>&, Previous,
	const TArray<FVector>&, Target);

UCLASS()
class AZULPROJECT_API AAzulHiloBase : public AActor, public IAzulHiloInterface
{
	GENERATED_BODY()

public:
	AAzulHiloBase();

protected:
	virtual void BeginPlay() override;

public:

	// Interfaz
	virtual void UpdateSpline_Implementation(const FVector& TriggerPos) override;

	// Llamado desde BP para aplicar puntos interpolados
	UFUNCTION(BlueprintCallable, Category = "Hilo")
	void ApplyInterpolatedSplinePoints(const TArray<FVector>& Points);

	// Evento para BP (Timeline)
	UPROPERTY(BlueprintAssignable, Category = "Hilo")
	FOnSplineRouteChanged OnSplineRouteChanged;

	// Componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Hilo")
	AAzulInteractuableBase* HijoActor = nullptr;


private:

	TArray<FVector> PreviousPoints;
	TArray<FVector> TargetPoints;

	FVector CachedStartPos;

	TArray<FVector> GenerateCurvedRoute(const FVector& StartPos, const FVector& EndPos);
};
