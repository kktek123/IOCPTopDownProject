// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "S1.h"
#include "Game/S1Player.h"
#include "Game/S1Monster.h"
#include "Game/S1Item.h"
#include "S1GameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatMessage, const FText&/*Message*/, msg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignInSuccess, const bool/*bool*/, success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignUpSuccess, const bool/*bool*/, success);
/**
 * 
 */
UCLASS()
class S1_API US1GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	US1GameInstance();
	~US1GameInstance();

	UFUNCTION(BlueprintCallable)
		void ConnectToGameServer();
	
	UFUNCTION(BlueprintCallable)
		void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
		void HandleRecvPackets();
	
	UFUNCTION(BlueprintCallable)
		void SendEnterInGame(FName LevelName);


	void SendPacket(SendBufferRef SendBuffer);

public:
	void HandleLogin(const Protocol::S_LOGIN& LoginPkt);
	void HandleLogout(const Protocol::S_LOGOUT& Pkt);
	void HandleCreateAccount(const Protocol::S_CREATEACCOUNT& Pkt);

	
	void HandleSpawn(const Protocol::PlayerInfo& ObjectInfo,bool IsMine);
	void SpawnMonster(const Protocol::MonsterInfo& ObjectInfo);
	void SpawnItem(const Protocol::ObjectItemInfo& ObjectInfo);

	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);
	void HandleSpawnPlayer(const Protocol::S_SPAWNPLAYER& SpawnPkt);
	void HandleSpawnMonster(const Protocol::S_SPAWNMONSTER& SpawnPkt);
	void HandleSpawnItem(const Protocol::S_SPAWNITEM& SpawnPkt);

	void HandleInventoryUpdate(const Protocol::S_INVENTORY_UPDATE& Pkt);
	void HandleInventoryRemove(const Protocol::S_INVENTORY_REMOVE& Pkt);


	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	void HandleDeath(const Protocol::S_DEATH& DeathPkt);

	UFUNCTION(BlueprintCallable)
	void HandleLeaveRoom();
	void HandleRefreshItemList();
	void HandleBuyShopItem(int shopindex);

	void HandleUpdateItemList(const Protocol::S_SHOP_ITEM_LIST& Pkt);
	void HandleUpdateMailSolt(const Protocol::S_MAIL_SEND& Pkt);
	void HandleUpdateMailList(const Protocol::S_MAIL_LIST& Pkt);
	void HandleSoldOutItem(const Protocol::S_BUY_SHOP_ITEM& Pkt);
	void HandleGoldUpdate(const Protocol::S_GOLD& Pkt);


	void HandleMove(const Protocol::S_MOVE& MovePkt);
	void HandleStatus(const Protocol::S_STATUS& StatusPkt);
	void MonsterMove(const Protocol::S_MOVE& MovePkt);
	void MonsterStatus(const Protocol::S_STATUS& StatusPkt);


	void HandleMeleeAttack(const Protocol::S_MELEEATTACK& atkPkt);
	void HandleShoot(const Protocol::S_SHOOTPROJECTILE& ShootPkt);
	void HandleHitDamage(const Protocol::S_HITDAMAGE& HitPkt);

public:
	void HandleSwapInventory(int fromIndex, int toIndex);

public:
	//GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

	class AS1MyPlayer* MyPlayer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FChatMessage OnChatMessage;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSignInSuccess OnSignIn;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSignUpSuccess OnSignUp;

	UFUNCTION(BlueprintCallable)
	void SendSignInAccount(FText id, FText passward);

	UFUNCTION(BlueprintCallable)
	void SendSignUpAccount(FText id, FText passward);

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AS1Player> OtherPlayerClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AS1Monster> OtherMonsterClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AS1Item> OtherItemClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> ItemShopClass;
	TMap<uint64, AS1Player*> Players;
	TMap<uint64, AS1Monster*> Monsters;
	TMap<uint64, AS1Item*> Items;

	bool bConnected;
public:
	UPROPERTY(BlueprintReadOnly)
	bool bSignPlayer;

};
