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
	Vector<int32>	_discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			_discoveredCount = 0; // 노드가 발견된 순서
	Vector<bool>	_finished; // Dfs(i)가 종료 되었는지 여부
	Vector<int32>	_parent;
};

