#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/*--------------
	Listener
---------------*/

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);//비동기 함수 등록
	void ProcessAccept(AcceptEvent* acceptEvent);//비동기 함수 연결

protected:
	SOCKET _socket = INVALID_SOCKET;
	//요청받은 AcceptEvents
	std::vector<AcceptEvent*> _acceptEvents;
	//서버역할을 맟은 객체
	ServerServiceRef _service;
};

