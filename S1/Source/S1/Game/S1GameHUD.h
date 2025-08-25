// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1GameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Menu;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Mail;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Shop;
	UPROPERTY(meta = (BindWidget))
	class US1ItemShop* ItemShop;
	UPROPERTY(meta = (BindWidget))
	class US1MailBoxWidget* MailBoxWidget;
	UPROPERTY(meta = (BindWidget))
	class US1HpBar* HpBar;
public:
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void MailBoxToggle();
	UFUNCTION(BlueprintCallable)
	void ItemShopToggle();
};
