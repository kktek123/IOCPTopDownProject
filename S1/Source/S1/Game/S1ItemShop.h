// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "S1ItemShop.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1ItemShop : public UUserWidget
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void NativeConstruct() override;

public:
	virtual void Init();
	void Open();

	UFUNCTION(BlueprintCallable)
		void ClickRefreshButton();
	void UpdateShopItemList(const Protocol::ShopInfo& Info);
	void ItemSoldOut(int Index);
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;
	UPROPERTY(meta = (BindWidget))
	class US1ItemBuyButton* ShopButton_0;
	UPROPERTY(meta = (BindWidget))
	class US1ItemBuyButton* ShopButton_1;
	UPROPERTY(meta = (BindWidget))
	class US1ItemBuyButton* ShopButton_2;
	UPROPERTY(meta = (BindWidget))
	class US1ItemBuyButton* ShopButton_3;
	UPROPERTY(meta = (BindWidget))
	class US1GoldWidget* GoldWidget;
};
