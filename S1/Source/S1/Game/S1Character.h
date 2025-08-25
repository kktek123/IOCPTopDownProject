// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "S1Character.generated.h"

UCLASS()
class S1_API AS1Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS1Character();
	virtual ~AS1Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	UFUNCTION(BlueprintCallable)
	bool IsMyPlayer();


	Protocol::MoveState GetMoveState() { return PosInfo->state(); }
	virtual void SetMoveState(Protocol::MoveState State);

	virtual void PlayAnimation(class UAnimMontage* Anim);
public:
	uint64_t GetObjectId() { return PosInfo->object_id(); }


	virtual void SetPlayerInfo(const Protocol::PosInfo& Info);
	virtual void SetDestInfo(const Protocol::PosInfo& Info);
	virtual void SetStatusInfo(const Protocol::StatusInfo& Info);
	virtual void ShootProjectile(const Protocol::ProjectileInfo& Info);
	virtual void MeleeAttack(const Protocol::MeleeAttackInfo& Info);
	void RecvGetDamage(const Protocol::HitInfo& Info);

	UPROPERTY(BlueprintReadOnly)
	float maxhp;
	UPROPERTY(BlueprintReadOnly)
	float hp;
	UPROPERTY(BlueprintReadOnly)
	float maxmp;
	UPROPERTY(BlueprintReadOnly)
	float mp;
	UPROPERTY(BlueprintReadOnly)
	float atk;
	UPROPERTY(BlueprintReadOnly)
	float def;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	class USoundCue* AttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	class USoundCue* DamageSound;


	float currentDamage = 0;

protected:
	class Protocol::PosInfo* PosInfo;
	class Protocol::PosInfo* DestInfo;
	class Protocol::StatusInfo* StatusInfo;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MeleeAttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DeathAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DamageBox;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SendGetDamage(uint64 CauserId, float Damage, FVector HitLocation);

	bool bDeath = false;
public:
	virtual void Death();
};
