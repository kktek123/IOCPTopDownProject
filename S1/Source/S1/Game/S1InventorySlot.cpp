// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1InventorySlot.h"
#include "Protocol.pb.h"
#include "S1GameInstance.h"
#include "Components/Image.h"
#include "Components/Border.h"

void US1InventorySlot::SendSwapSlot(int32 SwapIndex, FS1ItemStruct fromStruct)
{
	if (itemStruct.ItemData.item_id != 0)
		return;
	if (SlotItemType == EItemType::WEAPON &&
		fromStruct.ItemData.item_type != (uint8)EItemType::WEAPON)
		return;
	if (SlotItemType == EItemType::ARMOR)
	{
		if (fromStruct.ItemData.equip_type != (uint8)SlotEquipType)
			return;
	}
	Cast<US1GameInstance>(GetGameInstance())->HandleSwapInventory(SwapIndex, SlotIndex);
}

void US1InventorySlot::SetItemImage()
{
	if(itemStruct.Thumnail != nullptr)
		ItemImage->SetBrushFromTexture(itemStruct.Thumnail);
	else
		ItemImage->SetBrushFromTexture(DefaultTexture);
	SetRareImage();
}

void US1InventorySlot::SetRareImage()
{
	if(itemStruct.ItemData.item_rarity == 0)
		RareImage->SetBrushColor(FColor(0, 0, 0, (0.3 * 255)));
	else if(itemStruct.ItemData.item_rarity == 255)
		RareImage->SetBrushColor(FColor(0, 255, 0, (0.3 * 255)));
	else if(itemStruct.ItemData.item_rarity == 2)
		RareImage->SetBrushColor(FColor(0, 0, 255, (0.3 * 255)));
	else if(itemStruct.ItemData.item_rarity == 3)
		RareImage->SetBrushColor(FColor(255, 0, 255, (0.3 * 255)));
	else if(itemStruct.ItemData.item_rarity == 4)
		RareImage->SetBrushColor(FColor(255, 255, 0, (0.3 * 255)));
}
