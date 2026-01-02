#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"
#include "Actors/AzulStoryObjectBase.h"
#include "Interfaces/AzulInteractuableInterface.h"
#include "InputMappingContext.h"            
#include "EnhancedInputSubsystems.h" 
#include "AzulComponentes/AzulBolsoComponent.h"
#include "Actors/AzulHiloBase.h"
#include "AzulComponentes/AzulHiloComponent.h"
#include "InputAction.h"
#include "AzulCharacterBase.generated.h"


UENUM(BlueprintType)
enum class EAzulControlMode : uint8
{
	Default,
	Menu,
	Look,
	Disabled
};


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

	//-------------------------INTERACTUAR----------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Interactuable")
	TScriptInterface<IAzulInteractuableInterface> CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Azul|Interactuable")
	bool IsLookingAtInteractable(UCameraComponent* Camera) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool HasInteractable() const;

	//---------------------------BIBERÓN-----------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Biberon")
	bool bHasBiberon = false;

	//---------------------------------STORY--------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Story")
	FGameplayTagContainer ActiveStoryTags;

	UFUNCTION(BlueprintCallable, Category = "Azul|Story")
	void AddStoryTag(const FGameplayTag& NewTag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Azul|Story")
	FString SonName;

	//---------------------------BOLSO---------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Bolso")
	UAzulBolsoComponent* BolsoComponent;

	//---------------------------HILO--------------------------------------------

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Hilo")
	UAzulHiloComponent* HiloComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Hilo")
	AAzulHiloBase* HiloActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Azul|Input")
	UInputAction* IA_MostrarHilo;

	UFUNCTION(BlueprintImplementableEvent, Category = "Azul|Hilo")
	void BP_OnHiloShown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Azul|Hilo")
	void BP_OnHiloHidden();

	// Llamadas desde componentes
	void NotifyHiloShown();
	void NotifyHiloHidden();


	UFUNCTION()
	void OnSpacePressed();

	//----------------------------INPUT-------------------------------------------

	// Input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Control modes
	void SetControlMode(EAzulControlMode NewMode);

	// Estados
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Azul|Input")
	EAzulControlMode CurrentControlMode = EAzulControlMode::Default;

	// IMC preparados
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Azul|Input")
	TMap<EAzulControlMode, UInputMappingContext*> MappingContexts;

	//CONTROLES PARA CINEMÁTICAS
	void BlockPlayerControl();
	void UnblockPlayerControl();


private:
	bool bIsBlocked = false;

};
