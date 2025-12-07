#pragma once

#include "CoreMinimal.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "AzulAscensorBase.generated.h"


/**
 * 
 */
UCLASS()
class AZULPROJECT_API AAzulAscensorBase : public AAzulInteractuableBase
{
	GENERATED_BODY()

public:

	AAzulAscensorBase();

protected:
	virtual void BeginPlay() override;

public:
public:

    // Estado del movimiento
    bool bIsMoving = false;
    bool bIsUp = true;

    // Timer
    FTimerHandle MoveHandle;

    // Posiciones
    FVector StartLocation;
    FVector TargetLocationUp;
    FVector TargetLocationDown;

    float Speed = 200.f;

    // Movimiento
    void MoveTick();

    void ActivateIMCLook();
    void ActivateIMCDefault();


    virtual void Interactua_Implementation() override;
};
