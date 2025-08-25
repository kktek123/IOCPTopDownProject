// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1Item.h"
#include "S1ItemManager.h"

// Sets default values
AS1Item::AS1Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	ItemInfo = new Protocol::ItemInfo();
	PosInfo = new Protocol::PosInfo();
}

AS1Item::~AS1Item()
{
	delete ItemInfo;
	delete PosInfo;
}

void AS1Item::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//SetItemVisual();
}

// Called when the game starts or when spawned
void AS1Item::BeginPlay()
{
	Super::BeginPlay();
}

void AS1Item::SetItemVisual()
{
	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ItemData"), Assets);
	if (!Assets.IsEmpty())
	{
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[0]));
		if (AssetPtr.IsPending())
			AssetPtr.LoadSynchronous();
		if (US1ItemDataAsset* asset = Cast<US1ItemDataAsset>(AssetPtr.Get()))
		{
			if (FS1ItemStruct* item = asset->AssetItems.Find(ItemVisualName))
				itemStruct = *item;
		}
	}

	StaticMesh->SetStaticMesh(itemStruct.Mesh);
	StaticMesh->SetSimulatePhysics(false);
}

// Called every frame
void AS1Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS1Item::SetItemInfo(const Protocol::ItemInfo& Info)
{
	if (ItemInfo->object_id() != 0)
	{
		assert(ItemInfo->object_id() == Info.object_id());
	}
	ItemInfo->CopyFrom(Info);
	ItemVisualName = FString(ItemInfo->item_visual_name().c_str());
	SetItemVisual();
	itemStruct.ItemData.item_name = FString(ItemInfo->item_name().c_str());
	itemStruct.ItemData.item_type = (int32)ItemInfo->item_type();
	itemStruct.ItemData.item_rarity = (int32)ItemInfo->item_rarity();
	itemStruct.ItemData.custom_type = (int32)ItemInfo->custom_type();
	itemStruct.ItemData.equip_type = (int32)ItemInfo->equip_type();
	itemStruct.ItemData.atk = ItemInfo->atk();
	itemStruct.ItemData.def = ItemInfo->def();
	//itemData.Delay = ItemInfo->def();
	//itemData.Range = ItemInfo->def();
	ItemRarality = itemStruct.ItemData.item_rarity;
	SetNiagaraEffect();
}

void AS1Item::SetPosInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}
	PosInfo->CopyFrom(Info);
}


void AS1Item::SetNiagaraEffect_Implementation()
{
}

