// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1MailSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1MailSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateMailBox();
	class US1MailBoxWidget* MailBox;
	int selfindex;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainTitle;

};
