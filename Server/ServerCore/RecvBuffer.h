#pragma once

/*--------------
	RecvBuffer
----------------*/


//[r][][][w][][][][[][]
//��ȯ ����
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	//��ó�� �������� ������ ��ġ
	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	//������Ʈ�Ǵ� ������ ��ġ
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	//������Ʈ�Ǵ� ������ ũ��
	int32			DataSize() { return _writePos - _readPos; }
	//���� ������ ũ��
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	std::vector<BYTE>_buffer;
};

