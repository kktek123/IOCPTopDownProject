// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "S1ItemBuyButton.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1ItemBuyButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateItemData(const Protocol::ShopItemInfo& info);
	UFUNCTION(BlueprintCallable)
	void ClickBuyButton();
	uint32 GetShopItemId() { return ShopItemId; }
	uint32 GetShopItemIndex() { return ShopItemIndex; }
	void SetShopItemIndex(uint32 index) {  ShopItemIndex = index; }
	void ItemSoldOut();
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemPrice;
	UPROPERTY(meta = (BindWidget))
	class UButton* ItemBuyButton;
	UPROPERTY(meta = (BindWidget))
	class UBorder* SoldOutFilter;
private:
	uint32 ShopItemId;
	uint32 ShopItemIndex;
};
