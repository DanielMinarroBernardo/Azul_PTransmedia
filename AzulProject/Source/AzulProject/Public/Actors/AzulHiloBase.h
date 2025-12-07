#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Interfaces/AzulHiloInterface.h"
#include "AzulHiloBase.generated.h"

class AAzulCharacterBase;

UCLASS()
class AZULPROJECT_API AAzulHiloBase : public AActor, public IAzulHiloInterface
{
	GENERATED_BODY()

public:
	AAzulHiloBase();

protected:
	virtual void BeginPlay() override;

public:

	/** Llamado desde el trigger vía interfaz */
	virtual void UpdateSpline_Implementation(const FVector& TriggerPos) override;

	/** Aplica puntos al spline */
	void ApplySplinePoints(const TArray<FVector>& Points);

	/** Actualiza la tubería (SplineMesh) */
	void UpdateSplineMeshes();

	/** Genera una curva suave entre Player y HijoActor */
	TArray<FVector> GenerateCurvedRoute(const FVector& StartPos, const FVector& EndPos);

	// --------- COMPONENTES ---------

	UPROPERTY(VisibleAnywhere, Category = "Azul|Hilo")
	USplineComponent* SplineComp;

	/** Actor destino al que va el hilo */
	UPROPERTY(EditInstanceOnly, Category = "Azul|Hilo")
	AActor* HijoActor = nullptr;

	/** Mesh visual de la tubería */
	UPROPERTY(EditAnywhere, Category = "Azul|Hilo")
	UStaticMesh* TuboMesh;

	UPROPERTY(EditAnywhere, Category = "Azul|Hilo")
	UMaterialInterface* TuboMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Azul|Hilo")
	TArray<USplineMeshComponent*> SplineMeshes;

private:
	AAzulCharacterBase* CachedPlayer = nullptr;
};
