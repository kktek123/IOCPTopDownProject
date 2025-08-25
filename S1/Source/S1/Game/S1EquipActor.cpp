// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1EquipActor.h"

// Sets default values
AS1EquipActor::AS1EquipActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AS1EquipActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AS1EquipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS1EquipActor::Equip_Implementation(AActor* OwnerActor)
{
}

