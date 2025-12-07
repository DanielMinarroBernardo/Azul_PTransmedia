#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"
#include "Actors/AzulStoryObjectBase.h"
#include "Widgets/AzulWidgetBolsoBase.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "InputMappingContext.h"            
#include "EnhancedInputSubsystems.h"          
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Azul|Hilo")
	AAzulTriggerHiloBase* CurrentTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interactuable")
	TScriptInterface<IAzulInteractuableInterface> CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Azul|Interactuable")
	bool IsLookingAtInteractable(UCameraComponent* Camera, float MinDot = 0.75f) const;

	TSubclassOf<AActor> CurrentInteractableClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Story")
	FGameplayTagContainer ActiveStoryTags;

	UFUNCTION(BlueprintCallable, Category = "Azul|Story")
	void AddStoryTag(const FGameplayTag& NewTag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Azul|Story")
	FString SonName;

	//---------------------------BOLSO
	UPROPERTY()
	TArray<FItemData> Bolso;

	UPROPERTY()
	UAzulWidgetBolsoBase* BolsoWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Azul|Bolso")
	TSubclassOf<UAzulWidgetBolsoBase> BolsoWidgetClass;

	UPROPERTY()
	FItemData PendingBolsoItem;

	UPROPERTY()
	FVector PendingBolsoItemWorldLocation;

	UPROPERTY(EditAnywhere, Category = "Azul|Bolso")
	TSubclassOf<AAzulStoryObjectBase> ItemActorClass;

	UPROPERTY()
	AAzulStoryObjectBase* PendingPhysicalPick;

	// Inicializa inventario vacío
	void InitializeBolso();

	UFUNCTION(BlueprintCallable, Category = "Azul|Bolso")
	bool TryAddItem(AAzulStoryObjectBase* WorldItem);

	UFUNCTION()
	void HandleSwapConfirmed(int32 SlotIndex);

	UFUNCTION()
	void SpawnDroppedItem(const FItemData& Item, const FVector& Location);

	//INPUT

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateUIMode();
	void DeactivateUIMode();

public:

	void EnableMappingContext(FName Name, int32 Priority = 1);

	void DisableMappingContext(FName Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Input")
	TMap<FName, UInputMappingContext*> MappingContexts;

};
