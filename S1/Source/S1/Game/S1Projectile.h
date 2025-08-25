// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S1Projectile.generated.h"

UCLASS()
class S1_API AS1Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS1Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DamageSphere;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCauserId(int64 id) { CauserId = id; }
	int64 GetCauserId() { return CauserId; }
	void SetCauserAtk(float atk){ CauserAtk = atk; }
	int64 GetCauserAtk() { return CauserAtk; }
protected:
	int64 CauserId = 0;
	float CauserAtk = 0;
private:
	UFUNCTION(Category = "Damage")
	void OnMyDamageSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
