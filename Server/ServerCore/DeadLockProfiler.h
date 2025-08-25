#pragma once
#include <stack>
#include <map>
#include <vector>

/*--------------------
	DeadLockProfiler
---------------------*/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	HashMap<const char*, int32>	_nameToId;
	HashMap<int32, const char*>	_idToName;
	Map<int32, Set<int32>>				_lockHistory;

	Mutex _lock;

private:
	Vector<int32>	_discoveredOrder; // ��尡 �߰ߵ� ������ ����ϴ� �迭
	int32			_discoveredCount = 0; // ��尡 �߰ߵ� ����
	Vector<bool>	_finished; // Dfs(i)�� ���� �Ǿ����� ����
	Vector<int32>	_parent;
};

