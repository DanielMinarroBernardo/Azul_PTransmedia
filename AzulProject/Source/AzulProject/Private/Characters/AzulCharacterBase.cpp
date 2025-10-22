// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AzulCharacterBase.h"

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

// Called to bind functionality to input
void AAzulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

