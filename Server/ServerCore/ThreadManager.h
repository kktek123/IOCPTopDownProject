#pragma once

#include <thread>
#include <functional>

/*------------------
	ThreadManager
-------------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	//�񵿱� �۾����� ��Ƽ������� ó���մϴ�.
	void	Launch(std::function<void(void)> callback);
	void	Join();

	//������ ī��Ʈ�� �����ϰų� ���ҽ�ŵ�ϴ�.
	static void InitTLS();
	static void DestroyTLS();
	//Job�̳� JobTimer�� �۾����� ��Ƽ������� ó���մϴ�.
	static void DoGlobalQueueWork();
	static void DistributeReservedJobs();

private:
	//�����带 �۾��ϴ� ���ȿ��� lock�� �ɾ��ݴϴ�.
	std::mutex			_lock;
	std::vector<std::thread>	_threads;
};

