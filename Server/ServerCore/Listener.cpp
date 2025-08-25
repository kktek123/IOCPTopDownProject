#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

/*--------------
	Listener
---------------*/

Listener::~Listener()
{
	SocketUtils::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO

		delete(acceptEvent);
	}
}

bool Listener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	//연결등록을 위한 소켓을 생성합니다.
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	//코어에 들어온 IOCP를 등록합니다.
	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	//이미 사용중인 포트번호를 재사용할수있도록 소켓옵션을 조정합니다.
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	//버퍼에 데이터가 남아있어도 전송하지않고 바로 종료하도록 합니다.
	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	//소켓에 설정한 주소를 묶습니다.
	if (SocketUtils::Bind(_socket, _service->GetNetAddress()) == false)
		return false;

	//TCP 소켓을 listen 상태로 만듭니다.
	if (SocketUtils::Listen(_socket) == false)
		return false;

	//설정한 최대 세션만큼의 등록이벤트를 처리합니다.
	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = _service->CreateSession(); // Register IOCP
	//클라이언트와 연결되는 세션/소켓을 생성해준다.

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	/*LPFN_ACCEPTEX: Windows에서 IOCP의 ACCEPTEX에 접근하기위한 함수 포인터로
	 클라이언트의 연결을 수락하면서 동시에 데이터를 수신하고 주소 정보를 얻을 수 있는 함수이다.*/
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		// 실패하면 일단 다시 Accept 걸어준다
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = acceptEvent->session;

	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}