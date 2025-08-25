// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/S1InventoryComponent.h"
#include "S1InventoryTab.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1InventoryTab : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Tab_Image;
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox;
	UPROPERTY(BlueprintReadOnly)
	class US1InventoryComponent* Inventory;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventorySlotClass;
public:
	void LoadInventory();
};
