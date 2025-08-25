#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "S1.h"
#include "S1GameInstance.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_CREATEACCOUNT(PacketSessionRef& session, Protocol::S_CREATEACCOUNT& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleCreateAccount(pkt);
	}

	return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{

	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLogin(pkt);
	}

	//for (auto& Player : pkt.players())
	//{

	//}

	//for (int32 i = 0; i < pkt.players_size(); i++)
	//{
	//	const Protocol::PlayerInfo& Player = pkt.players(i);
	//}

	////로비에서 캐릭터 선택해서 인덱스 전송.
	//Protocol::C_ENTER_GAME EnterGamePkt;
	//EnterGamePkt.set_playerindex(0);
	//SEND_PACKET(EnterGamePkt);

	return true;
}

bool Handle_S_LOGOUT(PacketSessionRef& session, Protocol::S_LOGOUT& pkt)
{
	return false;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ENTER_ROOM& pkt)
{
	return false;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		// TODO : 게임 종료? 로비로?
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_SPAWNPLAYER(PacketSessionRef& session, Protocol::S_SPAWNPLAYER& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawnPlayer(pkt);
	}

	return true;
}

bool Handle_S_SPAWNMONSTER(PacketSessionRef& session, Protocol::S_SPAWNMONSTER& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawnMonster(pkt);
	}

	return true;
}


bool Handle_S_INVENTORY_UPDATE(PacketSessionRef& session, Protocol::S_INVENTORY_UPDATE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleInventoryUpdate(pkt);
		//GameInstance->MonsterStatus(pkt);
	}
	return true;
}

bool Handle_S_INVENTORY_REMOVE(PacketSessionRef& session, Protocol::S_INVENTORY_REMOVE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleInventoryRemove(pkt);
		//GameInstance->MonsterStatus(pkt);
	}
	return true;
}

bool Handle_S_SPAWNITEM(PacketSessionRef& session, Protocol::S_SPAWNITEM& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawnItem(pkt);
	}
	return true;
}

bool Handle_S_SPAWNBOX(PacketSessionRef& session, Protocol::S_SPAWNBOX& pkt)
{
	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}
	
	return true;
}

bool Handle_S_DEATH(PacketSessionRef& session, Protocol::S_DEATH& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDeath(pkt);
	}

	return true;
}


bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMove(pkt);
	}
	return true;
}

bool Handle_S_STATUS(PacketSessionRef& session, Protocol::S_STATUS& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleStatus(pkt);
		//GameInstance->MonsterStatus(pkt);
	}
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();
	return true;
}

bool Handle_S_MELEEATTACK(PacketSessionRef& session, Protocol::S_MELEEATTACK& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMeleeAttack(pkt);
		//GameInstance->MonsterStatus(pkt);
	}
	return true;
}

bool Handle_S_PATHFINDING(PacketSessionRef& session, Protocol::S_PATHFINDING& pkt)
{
	return false;
}

bool Handle_S_SHOOTPROJECTILE(PacketSessionRef& session, Protocol::S_SHOOTPROJECTILE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleShoot(pkt);
		//GameInstance->MonsterStatus(pkt);
	}
	return true;
}

bool Handle_S_HITDAMAGE(PacketSessionRef& session, Protocol::S_HITDAMAGE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleHitDamage(pkt);
	}
	return true;
}

bool Handle_S_SHOP_ITEM_LIST(PacketSessionRef& session, Protocol::S_SHOP_ITEM_LIST& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleUpdateItemList(pkt);
	}
	return true;
}

bool Handle_S_BUY_SHOP_ITEM(PacketSessionRef& session, Protocol::S_BUY_SHOP_ITEM& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleSoldOutItem(pkt);
	}
	return true;
}

bool Handle_S_MAIL_SEND(PacketSessionRef& session, Protocol::S_MAIL_SEND& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleUpdateMailSolt(pkt);
	}
	return true;
}

bool Handle_S_MAIL_LIST(PacketSessionRef& session, Protocol::S_MAIL_LIST& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleUpdateMailList(pkt);
	}
	return true;
}

bool Handle_S_GOLD(PacketSessionRef& session, Protocol::S_GOLD& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		//GameInstance->HandleShoot(pkt);
		GameInstance->HandleGoldUpdate(pkt);
	}
	return true;
}
