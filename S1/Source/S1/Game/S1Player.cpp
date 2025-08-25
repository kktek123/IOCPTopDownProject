// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Player.h"
#include "S1MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "S1GameInstance.h"
#include "S1Projectile.h"
#include "S1InventoryComponent.h"

// Sets default values
AS1Player::AS1Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	InventoryComponent = CreateDefaultSubobject<US1InventoryComponent>(TEXT("Inventory"));

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	HeadMesh->SetupAttachment(GetMesh());
	ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	ChestMesh->SetupAttachment(GetMesh());
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	LegMesh->SetupAttachment(GetMesh());
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	HandMesh->SetupAttachment(GetMesh());
	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	FeetMesh->SetupAttachment(GetMesh());
	WeaponMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponRight"));
	WeaponMesh_R->SetupAttachment(GetMesh());
	WeaponMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponLeft"));
	WeaponMesh_L->SetupAttachment(GetMesh());
	LingMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LingRight"));
	LingMesh_R->SetupAttachment(GetMesh());
	LingMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LingLeft"));
	LingMesh_L->SetupAttachment(GetMesh());
}

AS1Player::~AS1Player()
{
}

// Called when the game starts or when spawned
void AS1Player::BeginPlay()
{
	Super::BeginPlay();
}

void AS1Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AS1Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AS1Player::ShootProjectile(const Protocol::ProjectileInfo& Info)
{
	Super::ShootProjectile(Info);


	FVector SpawnLocation(Info.spawn_x(),Info.spawn_y(),Info.spawn_z());
	FRotator SpawnRotation(0, Info.shoot_yaw(), 0);
	if(!bDeath)
	PlayAnimation(AttackAnimMontage);

	if(AActor * actor = GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.OverrideLevel = actor->GetLevel();
		SpawnParameters.ObjectFlags |= RF_Transient;
		AS1Projectile* Projectile = Cast<AS1Projectile>(actor);
		if (Projectile == nullptr)
			return;
		Projectile->SetCauserId(Info.causer_id());
		Projectile->SetCauserAtk(Info.damage());
	}


}

void AS1Player::UpdateInventory(int index, const Protocol::ItemInfo& Info)
{
	InventoryComponent->UpdateInventory(index, Info);
	//Protocol::ItemInfo* ui = InventoryList->mutable_list(index);
	//ui->CopyFrom(Info);
}

void AS1Player::UpdateInventoryList(const Protocol::InventoryList& Info)
{
	InventoryComponent->UpdateInventoryList(Info);
}

void AS1Player::BuyShopItem(int index)
{
	InventoryComponent->BuyShopItem(index);
}

void AS1Player::PlayAnimation(UAnimMontage* Anim)
{
	
	if(UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);
	
	if(UAnimInstance * AnimInstance = (HeadMesh) ? HeadMesh->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);
	
	if (UAnimInstance * AnimInstance = (ChestMesh) ? ChestMesh->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);
	if (UAnimInstance* AnimInstance = (LegMesh) ? LegMesh->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);

	if (UAnimInstance* AnimInstance = (HandMesh) ? HandMesh->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);

	if (UAnimInstance* AnimInstance = (FeetMesh) ? HandMesh->GetAnimInstance() : nullptr)
		AnimInstance->Montage_Play(Anim, 1);

}

void AS1Player::MeleeAttack(const Protocol::MeleeAttackInfo& Info)
{
	Super::MeleeAttack(Info);
	PlayAnimation(MeleeAttackAnimMontage);
	currentDamage = Info.damage();
	SetActorRotation(FRotator(0, Info.yaw(),0));
}


float AS1Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void AS1Player::Death()
{
	Super::Death();
}

