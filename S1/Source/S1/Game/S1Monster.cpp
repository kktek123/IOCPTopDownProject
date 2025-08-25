// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Monster.h"
#include "S1MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "S1.h"
#include "S1Projectile.h"
#include "Engine/DamageEvents.h"

// Sets default values
AS1Monster::AS1Monster()
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

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(RootComponent);
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AS1Monster::OnDamageBoxOverlap);

}

AS1Monster::~AS1Monster()
{
}

// Called when the game starts or when spawned
void AS1Monster::BeginPlay()
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
void AS1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AS1Monster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageCauser != this)
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (AS1Projectile* Causeractor = Cast<AS1Projectile>(DamageCauser))
		{
			SendGetDamage(Causeractor->GetCauserId(), Damage, PointDamageEvent->HitInfo.ImpactNormal);
			Causeractor->Destroy();
		}
		else if (AS1Player* Enemy = Cast<AS1Player>(DamageCauser))
		{
			SendGetDamage(Enemy->GetObjectId(), Damage, PointDamageEvent->HitInfo.ImpactNormal);
		}
	}

	return Damage;
}

void AS1Monster::MeleeAttack(const Protocol::MeleeAttackInfo& Info)
{
	Super::MeleeAttack(Info);
	PlayAnimation(AttackAnimMontage);
	currentDamage = Info.damage();
}

void AS1Monster::PlayAnimation(UAnimMontage* Anim)
{
	{
		UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
		AnimInstance->Montage_Play(Anim, 1);
	}
}

void AS1Monster::Death()
{
	Super::Death();
	PlayAnimation(DeathAnimMontage);
	bDeath = true;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Destroy();
		}, 1.5f, false);

}

void AS1Monster::OnDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (AS1MyPlayer* Enemy = Cast<AS1MyPlayer>(OtherActor))
		{
			//Enemy->GetDamage(atk);
			TSubclassOf <UDamageType> DamageType;
			UGameplayStatics::ApplyPointDamage(Enemy, currentDamage, SweepResult.ImpactNormal, SweepResult, nullptr, this, DamageType);
		}

	}
}

