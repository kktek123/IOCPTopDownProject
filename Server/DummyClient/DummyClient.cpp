#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "ClientPacketHandler.h"
#include "Utils.h"

char sendData[] = "Hello World";
#define CLIENTCNT 100

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		cout << "OnConnected" << endl;
		
		Protocol::C_ENTER_GAME pkt;
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		// TODO : packetId 대역 체크
		ClientPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected" << endl;
	}
};

int main()
{
	ClientPacketHandler::Init();

	this_thread::sleep_for(1s);
	vector<ClientServiceRef> services;

	for (int i = 0; i < CLIENTCNT; i++)
	{
		ClientServiceRef service = make_shared<ClientService>(
			NetAddress(L"127.0.0.1", 7777),
			make_shared<IocpCore>(),
			[=]() { return make_shared<ServerSession>(); }, // TODO : SessionManager 등
			1);
		ASSERT_CRASH(service->Start());

		for (int32 i = 0; i < 2; i++)
		{
			GThreadManager->Launch([=]()
				{
					while (true)
					{
						service->GetIocpCore()->Dispatch();
					}
				});
		}
		services.push_back(service);
	}



	while (true)
	{
		for (int i = 0; i < CLIENTCNT; i++)
		{
			Protocol::C_MOVE pkt;
			Protocol::PosInfo* Info = pkt.mutable_info();
			Info->set_object_id(i + 1);
			Info->set_x(Utils::GetRandom(-1000.f, 1000.f));
			Info->set_y(Utils::GetRandom(-1000.f, 1000.f));
			Info->set_z(0.0f);
			Info->set_yaw(Utils::GetRandom(0.f, 100.f));
			Info->set_pitch(0);
			Info->set_state(Protocol::MOVE_STATE_RUN);
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
			services[i]->Broadcast(sendBuffer);
			if(i % 5 == 0)
				this_thread::sleep_for(0.1s);
		}


		this_thread::sleep_for(1s);
	}

	GThreadManager->Join();

}