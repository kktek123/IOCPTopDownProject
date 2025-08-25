// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1GoldWidget.h"
#include "Components/TextBlock.h"
#include "S1GameInstance.h"
#include "S1MyPlayer.h"

void US1GoldWidget::Init()
{
	if (AS1MyPlayer* player = Cast<US1GameInstance>(GetGameInstance())->MyPlayer)
	{
		player->OnGoldChanged.AddDynamic(this, &ThisClass::UpdateGoldText);
		UpdateGoldText(_Gold, player->GetPlayerGold());
	}
}

void US1GoldWidget::UpdateGoldText(int64 oldValue, int64 newValue)
{
	GoldText->SetText(FText::FromString(FString::FromInt(newValue)));

}
