// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1EquipmentTab.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1EquipmentTab : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* HeadSlot;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* ChestSlot;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* LegSlot;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* FeetSlot;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* HandSlot;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* WeaponSlot_R;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* WeaponSlot_L;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* RingSlot_R;
	UPROPERTY(meta = (BindWidget))
	class US1InventorySlot* RingSlot_L;
	UPROPERTY(BlueprintReadOnly)
	class US1InventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> EquipmentSlotClass;

public:
	void LoadInventory();
};
