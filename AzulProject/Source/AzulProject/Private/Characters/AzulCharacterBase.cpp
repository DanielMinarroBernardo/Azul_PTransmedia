// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AzulCharacterBase.h"
#include "Actors/AzulInteractuableBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/AzulInteractuableInterface.h"

// Sets default values
AAzulCharacterBase::AAzulCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAzulCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAzulCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAzulCharacterBase::AddStoryTag(const FGameplayTag& NewTag)
{
    ActiveStoryTags.AddTag(NewTag);
}

bool AAzulCharacterBase::IsLookingAtInteractable(UCameraComponent* Camera, float MinDot) const
{
    if (!Camera || !CurrentInteractable.GetObject())
        return false;

    // Convertir objeto de interfaz a Actor
    AActor* RawActor = Cast<AActor>(CurrentInteractable.GetObject());
    if (!RawActor)
        return false;

    // Asegurar que es derivado de AAzulInteractuableBase
    AAzulInteractuableBase* Interactable = Cast<AAzulInteractuableBase>(RawActor);
    if (!Interactable)
        return false;

    // Obtener punto objetivo (centro del Mesh)
    FVector TargetPoint = Interactable->MeshComp
        ? Interactable->MeshComp->Bounds.Origin
        : Interactable->GetActorLocation();

    // LookAt + Dot Product
    FVector ToTarget = (TargetPoint - Camera->GetComponentLocation()).GetSafeNormal();
    float Dot = FVector::DotProduct(Camera->GetForwardVector(), ToTarget);

    return Dot >= MinDot;
}

// Called to bind functionality to input
void AAzulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

