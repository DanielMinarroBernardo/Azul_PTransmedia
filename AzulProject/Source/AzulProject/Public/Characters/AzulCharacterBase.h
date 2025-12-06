#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"
#include "Actors/AzulInteractuableBase.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "AzulCharacterBase.generated.h"

class AAzulTriggerHiloBase;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Azul|Interactuable")
	AAzulTriggerHiloBase* CurrentTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interactuable")
	TScriptInterface<IAzulInteractuableInterface> CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Azul|Interactuable")
	bool IsLookingAtInteractable(UCameraComponent* Camera, float MinDot = 0.75f) const;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Story")
	FGameplayTagContainer ActiveStoryTags;

	UFUNCTION(BlueprintCallable, Category = "Story")
	void AddStoryTag(const FGameplayTag& NewTag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Story")
	FString SonName;



	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
