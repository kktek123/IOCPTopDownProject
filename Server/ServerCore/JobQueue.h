#pragma once
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

/*--------------
	JobQueue
---------------*/

class JobQueue : public std::enable_shared_from_this<JobQueue>
{
public:

	//함수 바로 실행
	void DoAsync(CallbackType&& callback)
	{
		Push(make_shared<Job>(std::move(callback)));
	}

	//매개 변수가 있는 함수 바로 실행
	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(make_shared<Job>(owner, memFunc, std::forward<Args>(args)...));
	}

	//함수를 나중에 실행
	void DoTimer(uint64 tickAfter, CallbackType&& callback)
	{
		JobRef job = make_shared<Job>(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	//매개 변수가 있는 함수를 나중에 실행
	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = make_shared<Job>(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	void					Push(JobRef job, bool pushOnly = false);
	void					Execute();

protected:
	// job에다가 Lock을 걸고 안정적이고 순차적으로 실행하기 위한 Queue
	LockQueue<JobRef>		_jobs;
	// 실행할 jobs의 개수를 정확하게 구하기 위한 원자값
	std::atomic<int32>		_jobCount = 0;
};

