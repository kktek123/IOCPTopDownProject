// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1Character.h"
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
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

// Sets default values
AS1Character::AS1Character()
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

	PosInfo = new Protocol::PosInfo();
	DestInfo = new Protocol::PosInfo();
	StatusInfo = new Protocol::StatusInfo();

}

AS1Character::~AS1Character()
{
	delete PosInfo;
	delete DestInfo;
	delete StatusInfo;
	PosInfo = nullptr;
	DestInfo = nullptr;
	StatusInfo = nullptr;
}

// Called when the game starts or when spawned
void AS1Character::BeginPlay()
{
	Super::BeginPlay();
	{
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);
		DestInfo->set_yaw(GetControlRotation().Yaw);
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

// Called every frame
void AS1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);

	}

	if (IsMyPlayer() == false)
	{

		const Protocol::MoveState State = PosInfo->state();
		if (State == Protocol::MOVE_STATE_RUN)
		{

			//FVector Location = GetActorLocation();
			//FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), Location.Z);

			//FVector MoveDir = (DestLocation - Location);
			//const float DistToDest = MoveDir.Length();
			//MoveDir.Normalize();

			//float MoveDist = (MoveDir * 600.f * DeltaTime).Length();
			//MoveDist = FMath::Min(MoveDist, DistToDest);
			//FVector NextLocation = Location + MoveDir * MoveDist;

			//SetActorLocation(NextLocation);

			SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
			AddMovementInput((FVector(DestInfo->x(), DestInfo->y(), GetActorLocation().Z) - GetActorLocation()));
			//AddMovementInput(GetActorForwardVector());
		}
		else if (State == Protocol::MOVE_STATE_SKILL)
		{
		}
		else
		{
		}
	}
}

bool AS1Character::IsMyPlayer()
{
	return Cast<AS1MyPlayer>(this) != nullptr;
}

void AS1Character::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->state() == State)
		return;

	PosInfo->set_state(State);
}

void AS1Character::PlayAnimation(UAnimMontage* Anim)
{
}

void AS1Character::SetPlayerInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}
	PosInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void AS1Character::SetDestInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(DestInfo->object_id() == Info.object_id());
	}

	//Dest에서 최종상태 복사
	DestInfo->CopyFrom(Info);

	//상태만 바로 적용하자
	SetMoveState(Info.state());
}

void AS1Character::SetStatusInfo(const Protocol::StatusInfo& Info)
{
	if (StatusInfo->object_id() != 0)
	{
		assert(StatusInfo->object_id() == Info.object_id());
	}

	//Dest에서 최종상태 복사
	StatusInfo->CopyFrom(Info);
	maxhp = Info.maxhp();
	hp = Info.hp();
	maxmp = Info.maxmp();
	mp = Info.mp();
	atk = Info.atk();
	def = Info.def();
}

void AS1Character::ShootProjectile(const Protocol::ProjectileInfo& Info)
{
	UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
}

void AS1Character::MeleeAttack(const Protocol::MeleeAttackInfo& Info)
{
	if (bDeath)
		return;
	UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
}

void AS1Character::RecvGetDamage(const Protocol::HitInfo& Info)
{
	float Damage = Info.damage();
	Damage = Damage - def > 0 ? Damage - def : 0;
	hp -= Damage;
	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());
	if (IsMyPlayer())
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hp %f / %f"), maxhp, hp));
}

void AS1Character::SendGetDamage(uint64 CauserId, float Damage, FVector HitLocation)
{
	if (CauserId == PosInfo->object_id())
		return;
	if (CauserId != Cast<US1GameInstance>(GetWorld()->GetGameInstance())->MyPlayer->PosInfo->object_id())
		return;
	//Damage = Damage - def > 0 ? Damage - def : 0;
	//hp -= Damage;
	float Pitch = 0;
	float Yaw = 0;
	UKismetMathLibrary::GetYawPitchFromVector(GetActorLocation() - HitLocation, Yaw, Pitch);
	Protocol::C_HITDAMAGE hitPkt;
	{
		Protocol::HitInfo* Info = hitPkt.mutable_info();

		Info->set_object_id(PosInfo->object_id());
		Info->set_causer_id(CauserId);
		Info->set_damage(Damage);
		Info->set_hit_yaw(Yaw);
		Info->set_hit_back(5);
		Info->set_hit_stun(1);
	}
	SEND_PACKET(hitPkt);
}

float AS1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}



void AS1Character::Death()
{

}

