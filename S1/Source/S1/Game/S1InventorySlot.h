// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/S1ItemManager.h"
#include "S1InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class S1_API US1InventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	struct FS1ItemStruct itemStruct;
	
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	int32 SlotIndex;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UBorder* RareImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* DefaultTexture;

public:
	UFUNCTION(BlueprintCallable)
	void SendSwapSlot(int32 SwapIndex, FS1ItemStruct fromStruct);
	UFUNCTION(BlueprintCallable)
	void SetItemImage();
	UFUNCTION(BlueprintCallable)
	void SetRareImage();

	EItemType SlotItemType;
	EEquipType SlotEquipType;
};
