// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1ItemManager.h"
#include "S1MailBoxWidget.generated.h"

USTRUCT()
struct FS1MailData
{
	GENERATED_USTRUCT_BODY()
	FText MainTitle;
	FText DetailText;
	TArray<FS1ItemStruct> MailItems;
	bool is_read;
};

UCLASS()
class S1_API US1MailBoxWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* GetItemButton;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Mail;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Item;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainTitle;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DetailText;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventorySlotClass;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> MailSlotClass;
	TArray<FS1MailData> Mails;
public:
	void UpdateMailSlot(FS1MailData Mail);
	void UpdateMailList(const Protocol::MailBoxInfo& MailBox);
	void SendUpdateMailList();
	void UpdateMailDetail(int selectIndex);

	UFUNCTION(BlueprintCallable)
	void RecvMailItem();

private:
	int mailselectIndex = 0;
};
