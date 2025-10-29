#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "AzulCharacterBase.generated.h"

UCLASS()
class AZULPROJECT_API AAzulCharacterBase : public ACharacter, public IAzulInteractuableInterface
{
	GENERATED_BODY()

public:
	AAzulCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactuable")
	TScriptInterface<IAzulInteractuableInterface> CurrentInteractable;

};
