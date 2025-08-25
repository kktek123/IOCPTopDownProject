// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S1EquipActor.generated.h"

UCLASS()
class S1_API AS1EquipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS1EquipActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//AActor* OwnerActor;
	UFUNCTION(BlueprintNativeEvent)
	void Equip(AActor* OwnerActor);
	virtual void Equip_Implementation(AActor* OwnerActor);
};
