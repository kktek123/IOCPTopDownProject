// Fill out your copyright notice in the Description page of Project Settings.


#include "S1GameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "Network/PacketSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "S1MyPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/S1ItemShop.h"
#include "Game/S1MailBoxWidget.h"
#include "Game/S1GameHUD.h"

US1GameInstance::US1GameInstance()
{
}

US1GameInstance::~US1GameInstance()
{
	DisconnectFromGameServer();
}

void US1GameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bConnected = Socket->Connect(*InternetAddr);

	if (bConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		//Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		//TEMP : Lobby에서 캐릭터 선택창
		{
			//Protocol::C_LOGIN Pkt;
			//SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			//SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}

}

void US1GameInstance::DisconnectFromGameServer()
{
	if (!bConnected)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	//Protocol::C_LEAVE_GAME LeavePkt;
	//SEND_PACKET(LeavePkt);
	if(bSignPlayer)
		HandleLeaveRoom();
	if (GameServerSession)
	{
		GameServerSession->Disconnect();
		GameServerSession.Reset();
		GameServerSession = nullptr;
	}

	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		Socket->Close();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
	bConnected = false;
}

void US1GameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void US1GameInstance::SendEnterInGame(FName LevelName)
{
	if (bConnected && bSignPlayer)
	{
		Protocol::C_ENTER_ROOM EnterGamePkt;
		FString room_name = LevelName.ToString();
		EnterGamePkt.set_room_name(TCHAR_TO_UTF8(*room_name));
		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(EnterGamePkt);
		SendPacket(SendBuffer);
	}
}

void US1GameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void US1GameInstance::HandleLogin(const Protocol::S_LOGIN& LoginPkt)
{
	OnSignIn.Broadcast(LoginPkt.success());
	bSignPlayer = true;
	////로비에서 캐릭터 선택해서 인덱스 전송.
	//playerinfos.Empty();
	//for (auto a : LoginPkt.players())
	//{
	//	playerinfos.Push(a);
	//}
}

void US1GameInstance::HandleLogout(const Protocol::S_LOGOUT& Pkt)
{
	if (bSignPlayer)
	{
		Players.Empty();
		Monsters.Empty();
		Items.Empty();
		DisconnectFromGameServer();
		//UGameplayStatics::OpenLevel(GetWorld(), FName(*FrontEndLevel), false);
	}
}

void US1GameInstance::HandleCreateAccount(const Protocol::S_CREATEACCOUNT& Pkt)
{
	OnSignUp.Broadcast(Pkt.success());
}

void US1GameInstance::HandleSpawn(const Protocol::PlayerInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AS1MyPlayer* Player = Cast<AS1MyPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;
		MyPlayer = Player;
		Player->SetPlayerInfo(ObjectInfo.pos_info());
		Player->SetStatusInfo(ObjectInfo.status_info());
		Player->UpdateInventoryList(ObjectInfo.inventory_list());
		Player->UpdatePlayerGold(ObjectInfo.gold());
		Players.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		AS1Player* Player = Cast<AS1Player>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		if (Player == nullptr)
			return;
		Player->SetPlayerInfo(ObjectInfo.pos_info());
		Player->SetStatusInfo(ObjectInfo.status_info());
		Player->UpdateInventoryList(ObjectInfo.inventory_list());
		Players.Add(ObjectInfo.object_id(), Player);
	}
}

void US1GameInstance::SpawnMonster(const Protocol::MonsterInfo& ObjectInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Monsters.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	AS1Monster* Monster = Cast<AS1Monster>(World->SpawnActor(OtherMonsterClass, &SpawnLocation));
	if (Monster == nullptr)
		return;
	Monster->SetPlayerInfo(ObjectInfo.pos_info());
	Monster->SetStatusInfo(ObjectInfo.status_info());
	Monsters.Add(ObjectInfo.object_id(), Monster);

}

void US1GameInstance::SpawnItem(const Protocol::ObjectItemInfo& ObjectInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Monsters.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	AS1Item* Item = Cast<AS1Item>(World->SpawnActor(OtherItemClass, &SpawnLocation));
	if (Item == nullptr)
		return;
	Item->SetItemInfo(ObjectInfo.info());
	Item->SetPosInfo(ObjectInfo.pos_info());
	Items.Add(ObjectInfo.object_id(), Item);
}

void US1GameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(),true);
}

void US1GameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	//for (auto& Player : SpawnPkt.players())
	//{
	//	HandleSpawn(Player,false);
	//}
	//for (auto& Monster : SpawnPkt.monsters())
	//{
	//	SpawnMonster(Monster);
	//}
}

void US1GameInstance::HandleSpawnPlayer(const Protocol::S_SPAWNPLAYER& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player,false);
	}
}

void US1GameInstance::HandleSpawnMonster(const Protocol::S_SPAWNMONSTER& SpawnPkt)
{
	for (auto& Monster : SpawnPkt.monsters())
	{
		SpawnMonster(Monster);
	}
}

void US1GameInstance::HandleSpawnItem(const Protocol::S_SPAWNITEM& SpawnPkt)
{
	for (auto& item : SpawnPkt.items())
	{
		SpawnItem(item);
	}
}

void US1GameInstance::HandleInventoryUpdate(const Protocol::S_INVENTORY_UPDATE& Pkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = Pkt.object_id();

	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		if (AS1Player* Player = (*FindActor))
		{
			for (int i = 0; i < Pkt.index().size(); i++)
			{
				int ItemId = Pkt.items(i).item_id();
				if (ItemId > 0)
				{
					Player->UpdateInventory(Pkt.index(i), Pkt.items(i));
				}
			}
		}

	}
}

void US1GameInstance::HandleInventoryRemove(const Protocol::S_INVENTORY_REMOVE& Pkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = Pkt.object_id();

	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		if (AS1MyPlayer* Player = Cast<AS1MyPlayer>((*FindActor)))
		{
			for (int i = 0; i < Pkt.index().size(); i++)
			{
				Protocol::ItemInfo info;
				info.set_object_id(ObjectId);
				Player->UpdateInventory(Pkt.index(i), info);
			}
		}

	}
}

void US1GameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;
	
	// TODO : Despawn
	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		if (FindActor == nullptr)
			return;

		Players.Remove(ObjectId);
		//if (MyPlayerId == ObjectId)
		//{
		//	//SignPlayer = false;
		//	Players.Empty();
		//	Monsters.Empty();
		//	Items.Empty();
		//}
		if (*FindActor != nullptr)
			World->DestroyActor(*FindActor);

	}
	if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		if (FindMonster == nullptr)
			return;
		Monsters.Remove(ObjectId);
		if (*FindMonster != nullptr)
			World->DestroyActor(*FindMonster);

	}
	if (AS1Item** Find1Item = Items.Find(ObjectId))
	{
		if (Find1Item == nullptr)
			return;
		Items.Remove(ObjectId);
		if (*Find1Item != nullptr)
			World->DestroyActor(*Find1Item);
	}

}

void US1GameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void US1GameInstance::HandleDeath(const Protocol::S_DEATH& DeathPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;
	const uint64 ObjectId = DeathPkt.object_ids();
	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		if (FindActor == nullptr)
			return;
		
		Players.Remove(ObjectId);

		if (*FindActor != nullptr)
			(*FindActor)->Death();

	}
	if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		if (FindMonster == nullptr)
			return;
		Monsters.Remove(ObjectId);

		if (*FindMonster != nullptr)
			(*FindMonster)->Death();

	}

}

void US1GameInstance::HandleLeaveRoom()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	//playerinfos[0] = MyPlayer->GetPlayerInfo();

	Players.Empty();
	Monsters.Empty();
	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);
}

void US1GameInstance::HandleRefreshItemList()
{
	Protocol::C_REFRESH_ITEM_LIST pkt;
	SEND_PACKET(pkt);
}

void US1GameInstance::HandleBuyShopItem(int shopindex)
{
	if(MyPlayer != nullptr)
	MyPlayer->BuyShopItem(shopindex);

}

void US1GameInstance::HandleUpdateItemList(const Protocol::S_SHOP_ITEM_LIST& Pkt)
{
	if(MyPlayer != nullptr)
	if(MyPlayer->GameHUDWidget != nullptr)
	MyPlayer->GameHUDWidget->ItemShop->UpdateShopItemList(Pkt.info());
}

void US1GameInstance::HandleUpdateMailSolt(const Protocol::S_MAIL_SEND& Pkt)
{
	if (MyPlayer != nullptr)
		if (MyPlayer->GameHUDWidget != nullptr)
		{
			FS1MailData Mail;
			Mail.MainTitle = FText::FromString(Pkt.mail().main_title().c_str());
			Mail.DetailText = FText::FromString(Pkt.mail().text().c_str());

			UAssetManager& Manager = UAssetManager::Get();
			TArray<FPrimaryAssetId> Assets;

			for (const Protocol::ItemInfo& Info : Pkt.mail().item())
			{
				FS1ItemStruct MailItem;
				if (Info.item_id() != 0)
				{
					Manager.GetPrimaryAssetIdList(TEXT("ItemData"), Assets);
					if (!Assets.IsEmpty())
					{
						FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[0]));
						if (AssetPtr.IsPending())
							AssetPtr.LoadSynchronous();
						if (US1ItemDataAsset* asset = Cast<US1ItemDataAsset>(AssetPtr.Get()))
						{
							if (FS1ItemStruct* item = asset->AssetItems.Find(FString(Info.item_visual_name().c_str())))
								MailItem = *item;
						}
					}

				}
				else
				{
					MailItem = FS1ItemStruct();
				}

				MailItem.ItemData.item_name = FString(Info.item_name().c_str());
				MailItem.ItemData.item_id = Info.item_id();
				MailItem.ItemData.item_type = (int32)Info.item_type();
				MailItem.ItemData.item_rarity = (int32)Info.item_rarity();
				MailItem.ItemData.custom_type = (int32)Info.custom_type();
				MailItem.ItemData.equip_type = (int32)Info.equip_type();
				MailItem.ItemData.atk = Info.atk();
				MailItem.ItemData.def = Info.def();
				MailItem.ItemData.Delay = Info.delay();
				MailItem.ItemData.Range = Info.range();

				Mail.MailItems.Add(MailItem);
			}
			


			MyPlayer->GameHUDWidget->MailBoxWidget->UpdateMailSlot(Mail);

		}
}

void US1GameInstance::HandleUpdateMailList(const Protocol::S_MAIL_LIST& Pkt)
{
	if (MyPlayer != nullptr)
		if (MyPlayer->GameHUDWidget != nullptr)
		{
			MyPlayer->GameHUDWidget->MailBoxWidget->UpdateMailList(Pkt.mailbox());
		}
}

void US1GameInstance::HandleSoldOutItem(const Protocol::S_BUY_SHOP_ITEM& Pkt)
{
	if (MyPlayer != nullptr)
		if (MyPlayer->GameHUDWidget != nullptr)
		{
			if (Pkt.success())
			{
				MyPlayer->GameHUDWidget->ItemShop->ItemSoldOut(Pkt.item_index());
			}
		}
}

void US1GameInstance::HandleGoldUpdate(const Protocol::S_GOLD& Pkt)
{
	if (MyPlayer != nullptr)
		MyPlayer->UpdatePlayerGold(Pkt.value());
}

void US1GameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();

	AS1Player** FindActor = Players.Find(ObjectId);
	if (FindActor != nullptr)
	{
		AS1Player* Player = (*FindActor);
		if (Player->IsMyPlayer())
			return;
		const Protocol::PosInfo& info = MovePkt.info();
		Player->SetDestInfo(info);
	}
	else
	{
		AS1Monster** FindMonster = Monsters.Find(ObjectId);
		if (FindMonster == nullptr)
			return;

		AS1Monster* Monster = (*FindMonster);
		const Protocol::PosInfo& info = MovePkt.info();
		Monster->SetDestInfo(info);
	}





}

void US1GameInstance::HandleStatus(const Protocol::S_STATUS& StatusPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = StatusPkt.info().object_id();

	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		AS1Player* Player = (*FindActor);

		const Protocol::StatusInfo& info = StatusPkt.info();
		Player->SetStatusInfo(info);
	}
	else if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		AS1Monster* Monster = (*FindMonster);

		const Protocol::StatusInfo& info = StatusPkt.info();
		Monster->SetStatusInfo(info);
	}

}

void US1GameInstance::MonsterMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();

	AS1Monster** FindActor = Monsters.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AS1Monster* Monster = (*FindActor);


	const Protocol::PosInfo& info = MovePkt.info();
	Monster->SetDestInfo(info);
}

void US1GameInstance::MonsterStatus(const Protocol::S_STATUS& StatusPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = StatusPkt.info().object_id();

	AS1Monster** FindActor = Monsters.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AS1Monster* Monster = (*FindActor);


	const Protocol::StatusInfo& info = StatusPkt.info();
	Monster->SetStatusInfo(info);
}

void US1GameInstance::HandleMeleeAttack(const Protocol::S_MELEEATTACK& atkPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = atkPkt.ability().object_id();
	const Protocol::MeleeAttackInfo& info  = atkPkt.ability();
	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		AS1Player* Player = (*FindActor);
		Player->MeleeAttack(info);
	}
	else if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		AS1Monster* Monster = (*FindMonster);

		Monster->MeleeAttack(info);

	}
}

void US1GameInstance::HandleShoot(const Protocol::S_SHOOTPROJECTILE& ShootPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = ShootPkt.shooterid();
	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		AS1Player* Player = (*FindActor);
		Player->SetActorRotation(FRotator(0, ShootPkt.shooter_yaw(), 0));
		for (const Protocol::ProjectileInfo& info : ShootPkt.info())
		{
			Player->ShootProjectile(info);

		}
	}
	else if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		AS1Monster* Monster = (*FindMonster);

		for (const Protocol::ProjectileInfo& info : ShootPkt.info())
		{
			Monster->ShootProjectile(info);

		}
	}
}

void US1GameInstance::HandleHitDamage(const Protocol::S_HITDAMAGE& HitPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = HitPkt.info().object_id();
	if (AS1Player** FindActor = Players.Find(ObjectId))
	{
		AS1Player* Player = (*FindActor);

		const Protocol::HitInfo& info = HitPkt.info();
		Player->RecvGetDamage(info);
	}
	else if (AS1Monster** FindMonster = Monsters.Find(ObjectId))
	{
		AS1Monster* Monster = (*FindMonster);

		const Protocol::HitInfo& info = HitPkt.info();
		Monster->RecvGetDamage(info);
	}
}

void US1GameInstance::HandleSwapInventory(int fromIndex, int toIndex)
{
	Protocol::C_SWAP_INVENTORY_TO_INVENTORY pkt;
	pkt.set_from_object_id(MyPlayer->GetObjectId());
	pkt.set_from_index(fromIndex);
	pkt.set_from_count(1);
	pkt.set_to_object_id(MyPlayer->GetObjectId());
	pkt.set_to_index(toIndex);
	SEND_PACKET(pkt);
}

void US1GameInstance::SendSignInAccount(FText id, FText passward)
{
	if (bConnected)
	{
		//if (this->IsMyPlayer())
		{
			Protocol::C_LOGIN Pkt;
			//현재 위치 정보
			{
				Pkt.set_playerid(TCHAR_TO_UTF8(*id.ToString()));
				Pkt.set_passward(TCHAR_TO_UTF8(*passward.ToString()));
			}
			SEND_PACKET(Pkt);
		}
	}
}

void US1GameInstance::SendSignUpAccount(FText id, FText passward)
{
	if (bConnected)
	{
		//if (this->IsMyPlayer())
		{
			Protocol::C_CREATEACCOUNT Pkt;
			//현재 위치 정보
			{
				Pkt.set_playerid(TCHAR_TO_UTF8(*id.ToString()));
				Pkt.set_passward(TCHAR_TO_UTF8(*passward.ToString()));
			}
			SEND_PACKET(Pkt);
		}
	}
}
