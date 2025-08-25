// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1Projectile.h"
#include "S1.h"
#include "S1Character.h"
#include "Components/SphereComponent.h"
#include "S1GameInstance.h"
#include "S1MyPlayer.h"

// Sets default values
AS1Projectile::AS1Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &AS1Projectile::OnMyDamageSphereOverlap);

}

// Called when the game starts or when spawned
void AS1Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AS1Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS1Projectile::OnMyDamageSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (CauserId == Cast<US1GameInstance>(GetGameInstance())->MyPlayer->GetObjectId())
		if (AS1Character* Enemy = Cast<AS1Character>(OtherActor))
		{
			//Enemy->GetDamage(atk);
			TSubclassOf <UDamageType> DamageType;
			UGameplayStatics::ApplyPointDamage(Enemy, CauserAtk, SweepResult.ImpactNormal, SweepResult, nullptr, this, DamageType);
		}

	}
}

