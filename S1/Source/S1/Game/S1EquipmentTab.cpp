// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1EquipmentTab.h"
#include "Game/S1InventorySlot.h"
#include "Game/S1InventoryTab.h"
#include "Components/WrapBox.h"

void US1EquipmentTab::LoadInventory()
{
	WrapBox->ClearChildren();

	int index = 0;
	HeadSlot->itemStruct = Inventory->Items[index];
	HeadSlot->SetItemImage();
	HeadSlot->SlotIndex = index;
	HeadSlot->SlotItemType = EItemType::ARMOR;
	HeadSlot->SlotEquipType = EEquipType::HELMET;
	index++;
	ChestSlot->itemStruct = Inventory->Items[index];
	ChestSlot->SetItemImage();
	ChestSlot->SlotIndex = index;
	ChestSlot->SlotItemType = EItemType::ARMOR;
	ChestSlot->SlotEquipType = EEquipType::CHEST;
	index++;
	LegSlot->itemStruct = Inventory->Items[index];
	LegSlot->SetItemImage();
	LegSlot->SlotIndex = index;
	LegSlot->SlotItemType = EItemType::ARMOR;
	LegSlot->SlotEquipType = EEquipType::LEGS;
	index++;
	HandSlot->itemStruct = Inventory->Items[index];
	HandSlot->SetItemImage();
	HandSlot->SlotIndex = index;
	HandSlot->SlotItemType = EItemType::ARMOR;
	HandSlot->SlotEquipType = EEquipType::HANDS;
	index++;
	FeetSlot->itemStruct = Inventory->Items[index];
	FeetSlot->SetItemImage();
	FeetSlot->SlotIndex = index;
	FeetSlot->SlotItemType = EItemType::ARMOR;
	FeetSlot->SlotEquipType = EEquipType::FOOT;
	index++;
	RingSlot_R->itemStruct = Inventory->Items[index];
	RingSlot_R->SetItemImage();
	RingSlot_R->SlotIndex = index;
	FeetSlot->SlotItemType = EItemType::ARMOR;
	index++;
	RingSlot_L->itemStruct = Inventory->Items[index];
	RingSlot_L->SetItemImage();
	RingSlot_L->SlotIndex = index;
	FeetSlot->SlotItemType = EItemType::ARMOR;
	index++;
	WeaponSlot_R->itemStruct = Inventory->Items[index];
	WeaponSlot_R->SetItemImage();
	WeaponSlot_R->SlotIndex = index;
	WeaponSlot_R->SlotItemType = EItemType::WEAPON;
	index++;
	WeaponSlot_L->itemStruct = Inventory->Items[index];
	WeaponSlot_L->SetItemImage();
	WeaponSlot_L->SlotIndex = index;
	WeaponSlot_L->SlotItemType = EItemType::WEAPON;
	index++;


	for (int i = index; i < Inventory->EquipmentSlotSize; i++)
	{
		US1InventorySlot* SlotWidget = Cast<US1InventorySlot>(CreateWidget(GetWorld(), EquipmentSlotClass));
		if (SlotWidget)
		{
			SlotWidget->itemStruct = Inventory->Items[i];
			SlotWidget->SlotIndex = i;
			SlotWidget->SetItemImage();
			WrapBox->AddChildToWrapBox(SlotWidget);

		}
	}
}
