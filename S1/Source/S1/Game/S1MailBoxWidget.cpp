// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1MailBoxWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/S1InventorySlot.h"
#include "Game/S1MailSlotWidget.h"
#include "Protocol.pb.h"
#include "S1.h"
#include "S1MyPlayer.h"
#include "S1InventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void US1MailBoxWidget::UpdateMailSlot(FS1MailData Mail)
{
	US1MailSlotWidget* SlotWidget = Cast<US1MailSlotWidget>(CreateWidget(GetWorld(), MailSlotClass));
	if (SlotWidget)
	{
		ScrollBox_Mail->AddChild(SlotWidget);
		ScrollBox_Mail->ScrollToEnd();
		Mails.Add(Mail);
		SlotWidget->selfindex = Mails.Num() - 1;
		SlotWidget->MailBox = this;
		SlotWidget->MainTitle->SetText(Mail.MainTitle);
		//SlotWidget->slotas
	}
}

void US1MailBoxWidget::UpdateMailList(const Protocol::MailBoxInfo& MailBox)
{
	ScrollBox_Mail->ClearChildren();
	Mails.Empty();

	for(const Protocol::MailInfo& mailInfo : MailBox.mail())
	{
		FS1MailData Mail;
		
		Mail.MainTitle = FText::FromString(UTF8_TO_TCHAR(mailInfo.main_title().c_str()));
		Mail.DetailText = FText::FromString(UTF8_TO_TCHAR(mailInfo.text().c_str()));
		Mail.is_read = mailInfo.is_read();
		UAssetManager& Manager = UAssetManager::Get();
		TArray<FPrimaryAssetId> Assets;

		for (const Protocol::ItemInfo& Info : mailInfo.item())
		{
			FS1ItemStruct MailItem;
			if (Info.item_id() != 0)
			{
				Manager.GetPrimaryAssetIdList(TEXT("ItemData"), Assets);
				if (!Assets.IsEmpty())
				{
					FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[0]));
					if (AssetPtr.IsPending())
						AssetPtr.LoadSynchronous();
					if (US1ItemDataAsset* asset = Cast<US1ItemDataAsset>(AssetPtr.Get()))
					{
						if (FS1ItemStruct* item = asset->AssetItems.Find(FString(Info.item_visual_name().c_str())))
							MailItem = *item;
					}
				}

			}
			else
			{
				MailItem = FS1ItemStruct();
			}

			MailItem.ItemData.item_name = FString(Info.item_name().c_str());
			MailItem.ItemData.item_id = Info.item_id();
			MailItem.ItemData.item_type = (int32)Info.item_type();
			MailItem.ItemData.item_rarity = (int32)Info.item_rarity();
			MailItem.ItemData.custom_type = (int32)Info.custom_type();
			MailItem.ItemData.equip_type = (int32)Info.equip_type();
			MailItem.ItemData.atk = Info.atk();
			MailItem.ItemData.def = Info.def();
			MailItem.ItemData.Delay = Info.delay();
			MailItem.ItemData.Range = Info.range();

			Mail.MailItems.Add(MailItem);
		}
		UpdateMailSlot(Mail);
	}

}

void US1MailBoxWidget::SendUpdateMailList()
{
	Protocol::C_MAIL_LIST pkt;
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);

}

void US1MailBoxWidget::UpdateMailDetail(int selectIndex)
{
	mailselectIndex = selectIndex;
	MainTitle->SetText(Mails[selectIndex].MainTitle);
	DetailText->SetText(Mails[selectIndex].DetailText);
	ScrollBox_Item->ClearChildren();
	for (int i = 0; i < Mails[selectIndex].MailItems.Num(); i++)
	{
		US1InventorySlot* SlotWidget = Cast<US1InventorySlot>(CreateWidget(GetWorld(), InventorySlotClass));
		if (SlotWidget)
		{
			SlotWidget->itemStruct = Mails[selectIndex].MailItems[i];
			SlotWidget->SetItemImage();
			ScrollBox_Item->AddChild(SlotWidget);
			UScrollBoxSlot* solt = UWidgetLayoutLibrary::SlotAsScrollBoxSlot(SlotWidget);
			solt->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			solt->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

		}
	}
	if(Mails[selectIndex].is_read)
	GetItemButton->SetVisibility(ESlateVisibility::Collapsed);
	else
	GetItemButton->SetVisibility(ESlateVisibility::Visible);

}

void US1MailBoxWidget::RecvMailItem()
{
	if (mailselectIndex == -1)
		return;
	Protocol::C_MAIL_ITEM_RECV pkt;
	pkt.set_mailindex(mailselectIndex);
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
	GetItemButton->SetVisibility(ESlateVisibility::Collapsed);
	Mails[mailselectIndex].is_read = true;
}
