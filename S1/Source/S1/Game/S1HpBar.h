// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S1HpBar.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1HpBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadwrite)
		float HpPercent = 0;
};
