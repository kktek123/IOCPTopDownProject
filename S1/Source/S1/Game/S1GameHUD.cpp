// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1GameHUD.h"
#include "Game/S1MailBoxWidget.h"
#include "Game/S1ItemShop.h"

void US1GameHUD::Init()
{
    ItemShop->Init();
}

void US1GameHUD::MailBoxToggle()
{
    if (MailBoxWidget->IsVisible())
    {
        MailBoxWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        MailBoxWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        MailBoxWidget->SendUpdateMailList();
    }
}

void US1GameHUD::ItemShopToggle()
{
    if (ItemShop->IsVisible())
    {
        ItemShop->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        ItemShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        ItemShop->Open();
    }
}
