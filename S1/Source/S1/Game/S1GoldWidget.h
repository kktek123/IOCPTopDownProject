// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1GoldWidget.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1GoldWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Called when the game starts
	virtual void Init();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UFUNCTION()
	void UpdateGoldText(int64 oldValue, int64 newValue);
private:
	int64 _Gold;
};
