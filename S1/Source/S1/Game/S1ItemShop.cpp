// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1ItemShop.h"
#include "Components/Button.h"
#include "S1ItemBuyButton.h"
#include "S1GameInstance.h"
#include "Game/S1GoldWidget.h"

void US1ItemShop::NativeConstruct()
{
	Super::NativeConstruct();
	ShopButton_0->SetShopItemIndex(0);
	ShopButton_1->SetShopItemIndex(1);
	ShopButton_2->SetShopItemIndex(2);
	ShopButton_3->SetShopItemIndex(3);
}

void US1ItemShop::Init()
{
	ClickRefreshButton();
}

void US1ItemShop::Open()
{
	GoldWidget->Init();
}

void US1ItemShop::ClickRefreshButton()
{
	Cast<US1GameInstance>(GetGameInstance())->HandleRefreshItemList();
}

void US1ItemShop::UpdateShopItemList(const Protocol::ShopInfo& Info)
{
	ShopButton_0->UpdateItemData(Info.list(0));
	ShopButton_1->UpdateItemData(Info.list(1));
	ShopButton_2->UpdateItemData(Info.list(2));
	ShopButton_3->UpdateItemData(Info.list(3));
}

void US1ItemShop::ItemSoldOut(int Index)
{
	switch (Index)
	{
	case 0:
		ShopButton_0->ItemSoldOut();
		break;
	case 1:
		ShopButton_1->ItemSoldOut();
		break;
	case 2:
		ShopButton_2->ItemSoldOut();
		break;
	case 3:
		ShopButton_3->ItemSoldOut();
		break;
	default:
		break;
	}
}
