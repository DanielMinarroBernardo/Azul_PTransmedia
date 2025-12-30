// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AzulNarrativeComponent.generated.h"

class UAzulDialogue;
class UDataTable;
class UUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZULPROJECT_API UAzulNarrativeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAzulNarrativeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /* ¿Este actor muestra UI de diálogo? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Narrative")
    bool bShowDialogueWidget = true;

    /* ¿Usa sistema de diálogo completo? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Narrative")
    bool bUseDialogueSystem = true;

    /* Texto simple (si NO usamos DataTable) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Narrative", meta = (MultiLine))
    FString SimpleText;

    /* Tabla de diálogo (opcional) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Narrative")
    UDataTable* DialogueTable;

    /* Widget a mostrar */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Azul|Narrative")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    /* Inicia la narrativa */
    UFUNCTION(BlueprintCallable, Category = "Azul|Narrative")
    void StartNarrative();

    /* Cierra la narrativa */
    UFUNCTION(BlueprintCallable, Category = "Azul|Narrative")
    void EndNarrative();

private:

    UPROPERTY()
    UAzulDialogue* DialogueInstance = nullptr;

    UPROPERTY()
    UUserWidget* WidgetInstance = nullptr;

    void StartSimpleText();
    void StartDialogueTable();
		
};
