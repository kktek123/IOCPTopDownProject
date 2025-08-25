// Fill out your copyright notice in the Description page of Project Settings.


#include "S1InventoryComponent.h"
#include "S1InventoryTab.h"
#include "S1EquipmentTab.h"
#include "S1Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "S1GameInstance.h"
#include "S1MyPlayer.h"
#include "S1Player.h"
#include "Protocol.pb.h"
#include "S1EquipActor.h"

// Sets default values for this component's properties
US1InventoryComponent::US1InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void US1InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    Items.SetNum(EquipmentSlotSize + ItemArraySize);


    if (InventoryWidgetClass)
    {
        InventoryWidget = Cast<US1InventoryTab>(CreateWidget(GetWorld(), InventoryWidgetClass));
        if (InventoryWidget)
        {
            InventoryWidget->Inventory = this;
            InventoryWidget->LoadInventory();
        }
    }
    if (EquipmentWidgetClass)
    {
        EquipmentWidget = Cast<US1EquipmentTab>(CreateWidget(GetWorld(), EquipmentWidgetClass));
        if (EquipmentWidget)
        {
            EquipmentWidget->Inventory = this;
            EquipmentWidget->LoadInventory();
        }
    }
}

void US1InventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}


// Called every frame
void US1InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void US1InventoryComponent::InventoryToggle()
{
    if (InventoryWidget->IsVisible())
    {
        InventoryWidget->RemoveFromParent();
    }
    else
    {
        InventoryWidget->AddToViewport();
    }
}

void US1InventoryComponent::EquipmentToggle()
{
    if (EquipmentWidget->IsVisible())
    {
        EquipmentWidget->RemoveFromParent();
    }
    else
    {
        EquipmentWidget->AddToViewport();
    }
}

void US1InventoryComponent::InteractItem()
{
    GetOwner()->GetActorLocation() - FVector(0, 0, 65);
    FVector StartLocation = GetOwner()->GetActorLocation() - FVector(0, 0, 65);
    FVector EndLocation = StartLocation; 
    ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
    TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> HitResult;

    UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(), // World context object
        StartLocation,
        EndLocation,
        SphereRadius,
        TraceChannel,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::Type::None,
        HitResult,
        true
    );

    for (const FHitResult &hitdata : HitResult)
    {
        if (AS1Item* item = Cast<AS1Item>(hitdata.GetActor()))
        {
            Protocol::C_TRY_PICK_ITEM_TO_INVENTORY pkt;
            for (int32 i = EquipmentSlotSize; i < Items.Num(); i++)
            {
                auto getitem = item->GetItemStruct();
                //int32 sum = getitem.Quanitity + Items[i].Quanitity;
                AS1MyPlayer* Player = Cast<AS1MyPlayer>(GetOwner());
                if (Items[i].ItemData.item_id == 0)
                {
                    pkt.set_item_id(item->ItemInfo->object_id());
                    pkt.set_to_object_id(Player->GetObjectId());
                    pkt.set_to_index(i);
                    break;
                }
            }
            SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
            Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
            break;
        }
    }

}

bool US1InventoryComponent::BuyShopItem(int index)
{
    Protocol::C_BUY_SHOP_ITEM pkt;
    pkt.set_item_index(index);
    pkt.set_full_inventory(true);
    for (int32 i = EquipmentSlotSize; i < Items.Num(); i++)
    {
        if (Items[i].ItemData.item_id == 0)
        {
            pkt.set_inventory_index(i);
            pkt.set_full_inventory(false);
            break;
        }
    }
    SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
    Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
    return true;
}

bool US1InventoryComponent::RecvMailItem(int index)
{
    Protocol::C_BUY_SHOP_ITEM pkt;
    pkt.set_item_index(index);
    pkt.set_full_inventory(true);
    for (int32 i = EquipmentSlotSize; i < Items.Num(); i++)
    {
        //int32 sum = getitem.Quanitity + Items[i].Quanitity;
        AS1MyPlayer* Player = Cast<AS1MyPlayer>(GetOwner());
        if (Items[i].ItemData.item_id == 0)
        {
            pkt.set_inventory_index(i);
            pkt.set_full_inventory(false);
            break;
        }
    }
    SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
    Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
    return true;
}

void US1InventoryComponent::UpdateInventory(int index, const Protocol::ItemInfo& Info)
{
    if (index >= EquipmentSlotSize + ItemArraySize)
        return;
    if (Info.item_id() != 0)
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
                if (FS1ItemStruct* item = asset->AssetItems.Find(FString(Info.item_visual_name().c_str())))
                    Items[index] = *item;
            }
        }

    }
    else
    {
        Items[index] = FS1ItemStruct();
    }

    Items[index].ItemData.item_name = FString(Info.item_name().c_str());
    Items[index].ItemData.item_id = Info.item_id();
    Items[index].ItemData.item_type = (int32)Info.item_type();
    Items[index].ItemData.item_rarity = (int32)Info.item_rarity();
    Items[index].ItemData.custom_type = (int32)Info.custom_type();
    Items[index].ItemData.equip_type = (int32)Info.equip_type();
    Items[index].ItemData.atk = Info.atk();
    Items[index].ItemData.def = Info.def();
    EquipUpdate(index);
    if (InventoryWidget)
        InventoryWidget->LoadInventory();
    if (EquipmentWidget)
        EquipmentWidget->LoadInventory();
}

void US1InventoryComponent::UpdateInventoryList(const Protocol::InventoryList& Info)
{
    for (int i = 0; i < Info.list().size(); i++)
    {
        UpdateInventory(i, Info.list(i));
    }
   
}

void US1InventoryComponent::EquipUpdate(int equipIndex)
{
    AS1Player* Player = Cast<AS1Player>(GetOwner());
    switch (equipIndex)
    {
    case 0:
        Player->HeadMesh->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->HeadMesh->SetLeaderPoseComponent(Player->GetMesh());
        break;
    case 1:
        Player->ChestMesh->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->ChestMesh->SetLeaderPoseComponent(Player->GetMesh());
        break;
    case 2:
        Player->LegMesh->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->LegMesh->SetLeaderPoseComponent(Player->GetMesh());
        break;
    case 3:
        Player->HandMesh->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->HandMesh->SetLeaderPoseComponent(Player->GetMesh());
        break;
    case 4:
        Player->FeetMesh->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->FeetMesh->SetLeaderPoseComponent(Player->GetMesh());
        break;
    case 7:
        if (EquipActor != nullptr)
            EquipActor->Destroy();
        EquipActor = Cast<AActor>(GetWorld()->SpawnActor(Items[equipIndex].EquipActorClass));

        if (AS1EquipActor* castEquipActor = Cast<AS1EquipActor>(EquipActor))
        {
            castEquipActor->Equip(Player);
        }

        //Player->WeaponMesh_R->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        //Player->WeaponMesh_R->SetWorldLocation(Player->WeaponMesh_R->GetSocketLocation(FName("b_Bn_Bow_Root")));
        //Player->WeaponMesh_R->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("MainHand"));
        break;
    case 8:
        Player->WeaponMesh_L->SetSkinnedAssetAndUpdate(Items[equipIndex].SKMesh, true);
        Player->WeaponMesh_L->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),FName("hand_l"));
        break;

    }
}


