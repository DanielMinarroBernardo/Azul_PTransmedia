#pragma once

#include "CoreMinimal.h"
#include "Actors/AzulInteractuableBase.h"
#include "Characters/AzulCharacterBase.h"
#include "AzulAscensorBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulAscensorBase : public AAzulInteractuableBase
{
    GENERATED_BODY()

public:
    AAzulAscensorBase();

protected:
    virtual void BeginPlay() override;

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

    // Velocidad
    float Speed = 200.f;

    // Movimiento
    void MoveTick();

    virtual void Interactua_Implementation() override;
};
