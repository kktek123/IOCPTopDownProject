// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "ClientPacketHandler.h"

PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
	ClientPacketHandler::Init();
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	//Socket->Recv();
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;
		PacketSessionRef ThisPtr = AsShared();
		ClientPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::SendPacket(SendBufferRef SendBuffer)
{
	SendPacketQueue.Enqueue(SendBuffer);
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Stop();
		RecvWorkerThread->Exit();
		RecvWorkerThread->Destroy();
		RecvWorkerThread.Reset();
		RecvWorkerThread = nullptr;
	}
	if (SendWorkerThread)
	{
		SendWorkerThread->Stop();
		SendWorkerThread->Exit();
		SendWorkerThread->Destroy();
		SendWorkerThread.Reset();
		SendWorkerThread = nullptr;
	}

}
