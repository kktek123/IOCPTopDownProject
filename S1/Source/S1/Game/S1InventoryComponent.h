// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/S1ItemManager.h"
#include "S1InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class S1_API US1InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	US1InventoryComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EquipmentSlotSize = 9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemArraySize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SphereRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FS1ItemStruct> Items;
	
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> EquipmentWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	class US1InventoryTab* InventoryWidget;
	UPROPERTY(BlueprintReadWrite)
	class US1EquipmentTab* EquipmentWidget;
public:
	UFUNCTION(BlueprintCallable)
	void InventoryToggle();
	UFUNCTION(BlueprintCallable)
	void EquipmentToggle();
	UFUNCTION(BlueprintCallable)
	void InteractItem();
	UFUNCTION(BlueprintCallable)
	bool BuyShopItem(int index);

	UFUNCTION(BlueprintCallable)
	bool RecvMailItem(int index);


	void UpdateInventory(int index, const Protocol::ItemInfo& Info);
	void UpdateInventoryList(const Protocol::InventoryList& Info);
	void EquipUpdate(int equipIndex);

	AActor* EquipActor = nullptr;
};
