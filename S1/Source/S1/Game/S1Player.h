// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S1Character.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "S1Player.generated.h"

UCLASS()
class S1_API AS1Player : public AS1Character
{
	GENERATED_BODY()

public:
	AS1Player();
	virtual ~AS1Player();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
public:
	virtual void ShootProjectile(const Protocol::ProjectileInfo& Info) override;
	void UpdateInventory(int index, const Protocol::ItemInfo& Info);
	void UpdateInventoryList(const Protocol::InventoryList& Info);
	void BuyShopItem(int index);

	virtual void PlayAnimation(class UAnimMontage* Anim) override;
	virtual void MeleeAttack(const Protocol::MeleeAttackInfo& Info) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ChestMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* LegMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FeetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* LingMesh_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* LingMesh_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh_R;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh_L;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,meta = (AllowPrivateAccess = "true"))
	class US1InventoryComponent* InventoryComponent;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	virtual void Death() override;

};
