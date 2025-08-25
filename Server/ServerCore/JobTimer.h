#pragma once

struct JobData
{
	JobData(std::weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{

	}

	std::weak_ptr<JobQueue>	owner;
	JobRef				job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

/*--------------
	JobTimer
---------------*/

class JobTimer
{
public:
	//_items에 job를 push합니다.
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	//_items에 우선순위 높은 job을 실행합니다.
	void			Distribute(uint64 now);
	void			Clear();

private:
	USE_LOCK;
	std::priority_queue<TimerItem>	_items; //빨리 실행되는 작업이 우선순위가 높습니다;
	//job을 실행할때 1Thread만 작업에 관여하도록 합니다.
	std::atomic<bool>				_distributing = false;
};

