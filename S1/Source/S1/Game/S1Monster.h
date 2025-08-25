// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S1Character.h"
#include "Protocol.pb.h"
#include "S1Monster.generated.h"

UCLASS()
class S1_API AS1Monster : public AS1Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS1Monster();
	virtual ~AS1Monster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void MeleeAttack(const Protocol::MeleeAttackInfo& Info) override;

	virtual void PlayAnimation(class UAnimMontage* Anim) override;
	virtual void Death() override;
private:
	UFUNCTION(Category = "Damage")
	void OnDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
