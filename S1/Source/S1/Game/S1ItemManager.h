// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataAsset.h"
#include "Protocol.pb.h"
#include "S1ItemManager.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEquipType : uint8
{
	NONE = 0,
	HELMET = 1,
	CHEST = 2,
	LEGS = 3,
	HANDS = 4,
	FOOT = 5,
	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE = 0,
	WEAPON = 1,
	ARMOR = 2,
	COIN = 3,
	POTION = 4,
	THROW = 5,
	Count	UMETA(Hidden)
};

USTRUCT()
struct FS1ItemData
{
	GENERATED_USTRUCT_BODY()
	int64 item_id;
	FString item_name;
	int32 item_type;
	int32 item_rarity;
	int32 custom_type;
	int32 equip_type;
	float atk;
	float def;
	float Delay;
	float Range;
};

USTRUCT(Atomic,BlueprintType)
struct FS1ItemStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Stackable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quanitity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumnail = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SKMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EquipActorClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMelee;
	FS1ItemData ItemData;
	//Protocol::ItemInfo* ItemInfo;
};



UCLASS()
class S1_API US1ItemDataAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FString,FS1ItemStruct> AssetItems;
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}
}; 

UCLASS(Config = Game)
class S1_API US1ItemManager : public UAssetManager
{
	GENERATED_BODY()

};
