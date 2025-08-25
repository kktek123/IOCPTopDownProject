// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1ItemBuyButton.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Game/S1ItemManager.h"
#include "S1GameInstance.h"

void US1ItemBuyButton::UpdateItemData(const Protocol::ShopItemInfo& info)
{
	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;

	const Protocol::ItemInfo& itemInfo = info.item();
	ShopItemId = itemInfo.item_id();
	Manager.GetPrimaryAssetIdList(TEXT("ItemData"), Assets);
	if (!Assets.IsEmpty())
	{
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[0]));
		if (AssetPtr.IsPending())
			AssetPtr.LoadSynchronous();
		if (US1ItemDataAsset* asset = Cast<US1ItemDataAsset>(AssetPtr.Get()))
		{
			if (FS1ItemStruct* item = asset->AssetItems.Find(itemInfo.item_visual_name().c_str()))
				if(ItemImage != nullptr)
				ItemImage->SetBrushFromTexture(item->Thumnail);
		}
	}
	ItemName->SetText(FText::FromString(itemInfo.item_name().c_str()));
	ItemPrice->SetText(FText::FromString(FString::FromInt(itemInfo.gold())));
	SoldOutFilter->SetVisibility(ESlateVisibility::Collapsed);
}

void US1ItemBuyButton::ClickBuyButton()
{
	Cast<US1GameInstance>(GetGameInstance())->HandleBuyShopItem(ShopItemIndex);
}

void US1ItemBuyButton::ItemSoldOut()
{
	SoldOutFilter->SetVisibility(ESlateVisibility::Visible);
}
