#pragma once

/*--------------
	RecvBuffer
----------------*/


//[r][][][w][][][][[][]
//순환 버퍼
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	//맨처음 쓰여지는 버퍼의 위치
	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	//업데이트되는 버퍼의 위치
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	//업데이트되는 버퍼의 크기
	int32			DataSize() { return _writePos - _readPos; }
	//남는 버퍼의 크기
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	std::vector<BYTE>_buffer;
};

