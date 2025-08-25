// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S1Player.h"
#include "Templates/SubclassOf.h"
#include "S1MyPlayer.generated.h"

/** Forward declaration to improve compiling times */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FS1Delegate_OnGoldChanged, int64, OldValue, int64, NewValue);


class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class S1_API AS1MyPlayer : public AS1Player
{
	GENERATED_BODY()
public:
	AS1MyPlayer();
	virtual ~AS1MyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnInputEnd();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void GotoMove(FVector Destination);

	void AttackStart();
	void AttackEnd();

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SetDestinationClickAction;
	
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> GameHUDWidgetClass;
public:
	UPROPERTY(BlueprintReadWrite)
	class US1GameHUD* GameHUDWidget;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	//Cache
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	//Dirty Dlag Test
	FVector2D LastDesiredInput;

	APlayerController* PlayerController;
private:
	FVector CachedDestination;

	bool IsMoving;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	
	bool bAttacking;
public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UNiagaraSystem* FXCursor;

public:
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerGold(int64 newValue);
	UPROPERTY(BlueprintAssignable)
	FS1Delegate_OnGoldChanged OnGoldChanged;

	void RecvMailItem(int index);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SendGetDamage(uint64 CauserId, float Damage, FVector HitLocation) override;
private:
	UFUNCTION(Category = "Damage")
		void OnMyDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	int64 PlayerGold = 0;
	UFUNCTION()
	void SetPlayerGold(int64 oldvalue, int64 newvalue);
public:
	Protocol::InventoryList* InventoryList;
	int64 GetPlayerGold() { return PlayerGold; }
};
