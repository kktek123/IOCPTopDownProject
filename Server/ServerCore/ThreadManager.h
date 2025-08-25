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

	//비동기 작업들을 멀티스레드로 처리합니다.
	void	Launch(std::function<void(void)> callback);
	void	Join();

	//스레드 카운트를 증가하거나 감소시킵니다.
	static void InitTLS();
	static void DestroyTLS();
	//Job이나 JobTimer의 작업들을 멀티스레드로 처리합니다.
	static void DoGlobalQueueWork();
	static void DistributeReservedJobs();

private:
	//스레드를 작업하는 동안에는 lock를 걸어줍니다.
	std::mutex			_lock;
	std::vector<std::thread>	_threads;
};

