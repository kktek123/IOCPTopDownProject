// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1MyPlayer.h"
#include "Game/S1Monster.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "S1.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "S1Projectile.h"
#include "S1GameHUD.h"
#include "Engine/DamageEvents.h"
#include "S1MailBoxWidget.h"
#include "S1InventoryComponent.h"
#include "S1HpBar.h"

// Sets default values
AS1MyPlayer::AS1MyPlayer()
{
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 


	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(RootComponent);
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AS1MyPlayer::OnMyDamageBoxOverlap);

	OnGoldChanged.AddDynamic(this, &ThisClass::SetPlayerGold);

}

AS1MyPlayer::~AS1MyPlayer()
{
	//delete InventoryList;
	//InventoryList = nullptr;
}

// Called when the game starts or when spawned
void AS1MyPlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(Controller);
	PlayerController->bShowMouseCursor = true;
	PlayerController->DefaultMouseCursor = EMouseCursor::Default;
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	if (GameHUDWidgetClass)
	{
		GameHUDWidget = Cast<US1GameHUD>(CreateWidget(GetWorld(), GameHUDWidgetClass));
		if (GameHUDWidget)
		{
			GameHUDWidget->Init();
			GameHUDWidget->AddToViewport();
		}
	}
}

void AS1MyPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Cast<US1GameInstance>(GetGameInstance())->DisconnectFromGameServer();
}

// Called every frame
void AS1MyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameHUDWidget)
	{
		GameHUDWidget->HpBar->HpPercent = hp / maxhp;
	}

	//Send판정
	bool ForceSendPacket = false;

	if (LastDesiredInput != DesiredInput || IsMoving || bAttacking)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}
	
	if(UAIBlueprintHelperLibrary::GetCurrentPath(Controller) != nullptr)
		ForceSendPacket = true;

	//State 정보
	if (DesiredInput != FVector2D::Zero() || UAIBlueprintHelperLibrary::GetCurrentPath(Controller))
	{
		if (!bAttacking)
			SetMoveState(Protocol::MOVE_STATE_RUN);
		else
			SetMoveState(Protocol::MOVE_STATE_SKILL);

	}
	else {
		if (bAttacking)
			SetMoveState(Protocol::MOVE_STATE_SKILL);
		else
			SetMoveState(Protocol::MOVE_STATE_IDLE);
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;
		//현재 위치 정보
		{
			Protocol::PosInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PosInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
		}
		SEND_PACKET(MovePkt);
	}

}

// Called to bind functionality to input
void AS1MyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AS1MyPlayer::AttackStart);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AS1MyPlayer::AttackEnd);
		
		//Attacking
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AS1MyPlayer::OnInputStarted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AS1MyPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AS1MyPlayer::OnInputEnd);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AS1MyPlayer::OnInputEnd);
		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AS1MyPlayer::Look);


		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AS1MyPlayer::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AS1MyPlayer::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AS1MyPlayer::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AS1MyPlayer::OnSetDestinationReleased);

	}
}


void AS1MyPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	if (bAttacking)
		return;
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FVector Direction = GetActorLocation();
		Direction.X += MovementVector.Y * 300;
		Direction.Y += MovementVector.X * 300;
		Direction.Z = -0.5;
		GotoMove(Direction);
	}
}

void AS1MyPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AS1MyPlayer::OnInputStarted()
{
	Controller->StopMovement();
	IsMoving = true;
}

void AS1MyPlayer::OnInputEnd()
{
	DesiredInput = FVector2D::Zero();
	IsMoving = false;
}

void AS1MyPlayer::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;

	if (bIsTouch)
	{
		bHitSuccessful = PlayerController->GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		GotoMove(Hit.Location);
	}



}

void AS1MyPlayer::GotoMove(FVector Destination)
{
	CachedDestination = Destination;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  *CachedDestination.ToString());
	// Move towards mouse pointer or touch
	DesiredMoveDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(DesiredMoveDirection, 1.0, false);
	DesiredInput = FVector2D(DesiredMoveDirection);
	//Cache
	{
		const FVector Location = GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}
}

void AS1MyPlayer::AttackStart()
{
	bAttacking = true;

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	float Pitch = 0;
	float Yaw = 0;
	UKismetMathLibrary::GetYawPitchFromVector(Hit.Location - GetActorLocation(), Yaw, Pitch);
	if (InventoryComponent->Items[7].ItemData.item_id == 0)
		return;
	if (InventoryComponent->Items[7].IsMelee)
	{
		Protocol::C_MELEEATTACK pkt;
		Protocol::MeleeAttackInfo* info = pkt.mutable_ability();
		info->set_object_id(GetObjectId());
		info->set_damage(StatusInfo->atk());
		info->set_yaw(Yaw);
		SEND_PACKET(pkt);

	}
	else
	{
		Protocol::C_SHOOTPROJECTILE pkt;
		Protocol::ProjectileInfo* projectile = pkt.add_info();
		projectile->set_spawn_x(GetActorLocation().X);
		projectile->set_spawn_y(GetActorLocation().Y);
		projectile->set_spawn_z(GetActorLocation().Z);
		projectile->set_shoot_yaw(Yaw);
		projectile->set_causer_id(PosInfo->object_id());
		projectile->set_damage(StatusInfo->atk());
		pkt.set_shooter_yaw(Yaw);
		SEND_PACKET(pkt);
	}
	//DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AS1MyPlayer::AttackEnd()
{
	bAttacking = false;
	//DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}


void AS1MyPlayer::OnSetDestinationReleased()
{
	// If it was a short press
	//if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	DesiredInput = FVector2D::Zero();
	FollowTime = 0.f;
}

void AS1MyPlayer::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AS1MyPlayer::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}


void AS1MyPlayer::UpdatePlayerGold(int64 newValue)
{
	if(OnGoldChanged.IsBound())
		OnGoldChanged.Broadcast(PlayerGold, newValue);
}



void AS1MyPlayer::RecvMailItem(int index)
{
	FS1MailData Mail = GameHUDWidget->MailBoxWidget->Mails[index];
	int invencount = 0;
	Protocol::C_BUY_SHOP_ITEM pkt;
	pkt.set_item_index(index);
	pkt.set_full_inventory(true);
	for (int32 i = InventoryComponent->EquipmentSlotSize; i < InventoryComponent->Items.Num(); i++)
	{
		if (InventoryComponent->Items[i].ItemData.item_id == 0)
		{
			invencount++;
		}
	}
	for (int32 i = 0; i < GameHUDWidget->MailBoxWidget->Mails[index].MailItems.Num(); i++)
	{
		if (GameHUDWidget->MailBoxWidget->Mails[index].MailItems[i].ItemData.item_id != 0)
		{
			invencount++;
		}
	}
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
}

float AS1MyPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageCauser != this)
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (AS1Projectile* Causeractor = Cast<AS1Projectile>(DamageCauser))
		{
			if (Causeractor->GetCauserId() != GetObjectId())
			{
				SendGetDamage(Causeractor->GetCauserId(), Damage, PointDamageEvent->HitInfo.ImpactNormal);
				Causeractor->Destroy();
			}

		}
		else if (AS1Monster* Enemy = Cast<AS1Monster>(DamageCauser))
		{
			SendGetDamage(Enemy->GetObjectId(), Damage, PointDamageEvent->HitInfo.ImpactNormal);
		}
	}

	return Damage;
}

void AS1MyPlayer::SendGetDamage(uint64 CauserId, float Damage, FVector HitLocation)
{
	if (CauserId == PosInfo->object_id())
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


void AS1MyPlayer::OnMyDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (AS1Monster* Enemy = Cast<AS1Monster>(OtherActor))
		{
			//Enemy->GetDamage(atk);
			TSubclassOf <UDamageType> DamageType;
			UGameplayStatics::ApplyPointDamage(Enemy, atk, SweepResult.ImpactNormal, SweepResult, nullptr,this, DamageType);
		}

	}
}

void AS1MyPlayer::SetPlayerGold(int64 oldvalue, int64 newvalue)
{
	if (oldvalue == PlayerGold)
		PlayerGold = newvalue;
}
