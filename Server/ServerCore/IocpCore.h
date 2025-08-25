#pragma once

/*----------------
	IocpObject
-----------------*/

class IocpObject : public std::enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	//IOCP 식별을 위해 사용하는 추상적 포인터
	HANDLE		GetHandle() { return _iocpHandle; }

	//IOCP 등록
	bool		Register(IocpObjectRef iocpObject);
	//IOCP 연결/입출력 처리
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};