// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1InventoryTab.h"
#include "Game/S1InventorySlot.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"

void US1InventoryTab::LoadInventory()
{
	WrapBox->ClearChildren();

	for (int i = Inventory->EquipmentSlotSize; i < Inventory->EquipmentSlotSize + Inventory->ItemArraySize; i++)
	{
		US1InventorySlot* SlotWidget = Cast<US1InventorySlot>(CreateWidget(GetWorld(), InventorySlotClass));
		if (SlotWidget)
		{
			SlotWidget->itemStruct = Inventory->Items[i];
			SlotWidget->SlotIndex = i;
			SlotWidget->SetItemImage();
			WrapBox->AddChildToWrapBox(SlotWidget);

		}
	}
}
