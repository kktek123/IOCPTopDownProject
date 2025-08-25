// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1MailSlotWidget.h"
#include "Game/S1MailBoxWidget.h"

void US1MailSlotWidget::UpdateMailBox()
{
	if(MailBox != nullptr)
	MailBox->UpdateMailDetail(selfindex);
}
