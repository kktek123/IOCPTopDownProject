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
	//_items�� job�� push�մϴ�.
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	//_items�� �켱���� ���� job�� �����մϴ�.
	void			Distribute(uint64 now);
	void			Clear();

private:
	USE_LOCK;
	std::priority_queue<TimerItem>	_items; //���� ����Ǵ� �۾��� �켱������ �����ϴ�;
	//job�� �����Ҷ� 1Thread�� �۾��� �����ϵ��� �մϴ�.
	std::atomic<bool>				_distributing = false;
};

