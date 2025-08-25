#pragma once
#include "Protocol.pb.h"

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "S1.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_CREATEACCOUNT = 1000,
	PKT_S_CREATEACCOUNT = 1001,
	PKT_C_LOGIN = 1002,
	PKT_S_LOGIN = 1003,
	PKT_C_LOGOUT = 1004,
	PKT_S_LOGOUT = 1005,
	PKT_C_ENTER_GAME = 1006,
	PKT_S_ENTER_GAME = 1007,
	PKT_C_ENTER_ROOM = 1008,
	PKT_S_ENTER_ROOM = 1009,
	PKT_C_LEAVE_GAME = 1010,
	PKT_S_LEAVE_GAME = 1011,
	PKT_S_SPAWN = 1012,
	PKT_S_SPAWNPLAYER = 1013,
	PKT_S_SPAWNMONSTER = 1014,
	PKT_C_DROPITEM = 1015,
	PKT_C_INVENTORY_UPDATE = 1016,
	PKT_S_INVENTORY_UPDATE = 1017,
	PKT_C_INVENTORY_REMOVE = 1018,
	PKT_S_INVENTORY_REMOVE = 1019,
	PKT_C_SWAP_INVENTORY_TO_INVENTORY = 1020,
	PKT_C_TRY_PICK_ITEM_TO_INVENTORY = 1021,
	PKT_S_SPAWNITEM = 1022,
	PKT_S_SPAWNBOX = 1023,
	PKT_S_DESPAWN = 1024,
	PKT_S_DEATH = 1025,
	PKT_C_MOVE = 1026,
	PKT_S_MOVE = 1027,
	PKT_C_STATUS = 1028,
	PKT_S_STATUS = 1029,
	PKT_C_CHAT = 1030,
	PKT_S_CHAT = 1031,
	PKT_S_MELEEATTACK = 1032,
	PKT_C_MELEEATTACK = 1033,
	PKT_C_PATHFINDING = 1034,
	PKT_S_PATHFINDING = 1035,
	PKT_C_SHOOTPROJECTILE = 1036,
	PKT_S_SHOOTPROJECTILE = 1037,
	PKT_C_HITDAMAGE = 1038,
	PKT_S_HITDAMAGE = 1039,
	PKT_S_SHOP_ITEM_LIST = 1040,
	PKT_C_REFRESH_ITEM_LIST = 1041,
	PKT_C_BUY_SHOP_ITEM = 1042,
	PKT_S_BUY_SHOP_ITEM = 1043,
	PKT_S_MAIL_SEND = 1044,
	PKT_S_MAIL_LIST = 1045,
	PKT_C_MAIL_LIST = 1046,
	PKT_C_MAIL_ITEM_RECV = 1047,
	PKT_S_GOLD = 1048,
	PKT_C_GOLD = 1049,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_CREATEACCOUNT(PacketSessionRef& session, Protocol::S_CREATEACCOUNT& pkt);
bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_LOGOUT(PacketSessionRef& session, Protocol::S_LOGOUT& pkt);
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ENTER_ROOM& pkt);
bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_SPAWNPLAYER(PacketSessionRef& session, Protocol::S_SPAWNPLAYER& pkt);
bool Handle_S_SPAWNMONSTER(PacketSessionRef& session, Protocol::S_SPAWNMONSTER& pkt);
bool Handle_S_INVENTORY_UPDATE(PacketSessionRef& session, Protocol::S_INVENTORY_UPDATE& pkt);
bool Handle_S_INVENTORY_REMOVE(PacketSessionRef& session, Protocol::S_INVENTORY_REMOVE& pkt);
bool Handle_S_SPAWNITEM(PacketSessionRef& session, Protocol::S_SPAWNITEM& pkt);
bool Handle_S_SPAWNBOX(PacketSessionRef& session, Protocol::S_SPAWNBOX& pkt);
bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_DEATH(PacketSessionRef& session, Protocol::S_DEATH& pkt);
bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_STATUS(PacketSessionRef& session, Protocol::S_STATUS& pkt);
bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);
bool Handle_S_MELEEATTACK(PacketSessionRef& session, Protocol::S_MELEEATTACK& pkt);
bool Handle_S_PATHFINDING(PacketSessionRef& session, Protocol::S_PATHFINDING& pkt);
bool Handle_S_SHOOTPROJECTILE(PacketSessionRef& session, Protocol::S_SHOOTPROJECTILE& pkt);
bool Handle_S_HITDAMAGE(PacketSessionRef& session, Protocol::S_HITDAMAGE& pkt);
bool Handle_S_SHOP_ITEM_LIST(PacketSessionRef& session, Protocol::S_SHOP_ITEM_LIST& pkt);
bool Handle_S_BUY_SHOP_ITEM(PacketSessionRef& session, Protocol::S_BUY_SHOP_ITEM& pkt);
bool Handle_S_MAIL_SEND(PacketSessionRef& session, Protocol::S_MAIL_SEND& pkt);
bool Handle_S_MAIL_LIST(PacketSessionRef& session, Protocol::S_MAIL_LIST& pkt);
bool Handle_S_GOLD(PacketSessionRef& session, Protocol::S_GOLD& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_CREATEACCOUNT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATEACCOUNT>(Handle_S_CREATEACCOUNT, session, buffer, len); };
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_S_LOGOUT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGOUT>(Handle_S_LOGOUT, session, buffer, len); };
		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_ENTER_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_ROOM>(Handle_S_ENTER_ROOM, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWNPLAYER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWNPLAYER>(Handle_S_SPAWNPLAYER, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWNMONSTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWNMONSTER>(Handle_S_SPAWNMONSTER, session, buffer, len); };
		GPacketHandler[PKT_S_INVENTORY_UPDATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_INVENTORY_UPDATE>(Handle_S_INVENTORY_UPDATE, session, buffer, len); };
		GPacketHandler[PKT_S_INVENTORY_REMOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_INVENTORY_REMOVE>(Handle_S_INVENTORY_REMOVE, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWNITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWNITEM>(Handle_S_SPAWNITEM, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWNBOX] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWNBOX>(Handle_S_SPAWNBOX, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DEATH] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DEATH>(Handle_S_DEATH, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_STATUS] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_STATUS>(Handle_S_STATUS, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_MELEEATTACK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MELEEATTACK>(Handle_S_MELEEATTACK, session, buffer, len); };
		GPacketHandler[PKT_S_PATHFINDING] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PATHFINDING>(Handle_S_PATHFINDING, session, buffer, len); };
		GPacketHandler[PKT_S_SHOOTPROJECTILE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SHOOTPROJECTILE>(Handle_S_SHOOTPROJECTILE, session, buffer, len); };
		GPacketHandler[PKT_S_HITDAMAGE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HITDAMAGE>(Handle_S_HITDAMAGE, session, buffer, len); };
		GPacketHandler[PKT_S_SHOP_ITEM_LIST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SHOP_ITEM_LIST>(Handle_S_SHOP_ITEM_LIST, session, buffer, len); };
		GPacketHandler[PKT_S_BUY_SHOP_ITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BUY_SHOP_ITEM>(Handle_S_BUY_SHOP_ITEM, session, buffer, len); };
		GPacketHandler[PKT_S_MAIL_SEND] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MAIL_SEND>(Handle_S_MAIL_SEND, session, buffer, len); };
		GPacketHandler[PKT_S_MAIL_LIST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MAIL_LIST>(Handle_S_MAIL_LIST, session, buffer, len); };
		GPacketHandler[PKT_S_GOLD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GOLD>(Handle_S_GOLD, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::C_CREATEACCOUNT& pkt) { return MakeSendBuffer(pkt, PKT_C_CREATEACCOUNT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGOUT& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGOUT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_DROPITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_DROPITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_INVENTORY_UPDATE& pkt) { return MakeSendBuffer(pkt, PKT_C_INVENTORY_UPDATE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_INVENTORY_REMOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_INVENTORY_REMOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SWAP_INVENTORY_TO_INVENTORY& pkt) { return MakeSendBuffer(pkt, PKT_C_SWAP_INVENTORY_TO_INVENTORY); }
	static SendBufferRef MakeSendBuffer(Protocol::C_TRY_PICK_ITEM_TO_INVENTORY& pkt) { return MakeSendBuffer(pkt, PKT_C_TRY_PICK_ITEM_TO_INVENTORY); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_STATUS& pkt) { return MakeSendBuffer(pkt, PKT_C_STATUS); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MELEEATTACK& pkt) { return MakeSendBuffer(pkt, PKT_C_MELEEATTACK); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PATHFINDING& pkt) { return MakeSendBuffer(pkt, PKT_C_PATHFINDING); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SHOOTPROJECTILE& pkt) { return MakeSendBuffer(pkt, PKT_C_SHOOTPROJECTILE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_HITDAMAGE& pkt) { return MakeSendBuffer(pkt, PKT_C_HITDAMAGE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_REFRESH_ITEM_LIST& pkt) { return MakeSendBuffer(pkt, PKT_C_REFRESH_ITEM_LIST); }
	static SendBufferRef MakeSendBuffer(Protocol::C_BUY_SHOP_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_BUY_SHOP_ITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MAIL_LIST& pkt) { return MakeSendBuffer(pkt, PKT_C_MAIL_LIST); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MAIL_ITEM_RECV& pkt) { return MakeSendBuffer(pkt, PKT_C_MAIL_ITEM_RECV); }
	static SendBufferRef MakeSendBuffer(Protocol::C_GOLD& pkt) { return MakeSendBuffer(pkt, PKT_C_GOLD); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = std::make_shared<SendBuffer>(packetSize);
#endif
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};