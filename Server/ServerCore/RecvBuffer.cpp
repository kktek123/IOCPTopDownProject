#include "pch.h"
#include "RecvBuffer.h"

/*--------------
	RecvBuffer
----------------*/

//TCP의 특성상 명시된 크기가 그대로 온다는 보장이 없다.
//패킷이 단편화 되어서 올 가능성이 있기 때문에 받을 때마다 초기화 해서 사용하면 기존 내용을 덮어쓰게 될 것이고 기존 내용은 지워진다. 그래서 받을때마다 위치를 갱신해서 완전히 받을수 있도록 해야 한다.
RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	if (dataSize == 0)
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		_readPos = _writePos = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		if (FreeSize() < _bufferSize)
		{
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}