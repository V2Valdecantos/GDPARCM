#pragma once
#include <queue>
#include <unordered_map>
#include "IETThread.h"
#include "IFinishedTask.h"

class PoolWorkerThread;
class IWorkerThread;
class IFinishedTask;
class ThreadPool : public IETThread, public IFinishedTask
{
private:
	typedef std::string String;
	typedef std::queue<PoolWorkerThread*> ThreadList;
	typedef std::unordered_map<int, PoolWorkerThread*> ActiveThreadMap;
	typedef std::queue<IWorkerAction*> ActionList;

public:
	ThreadPool(String name, int numWorkers);
	~ThreadPool();

	void startScheduler();
	void stopScheduler();
	void scheduleTask(IWorkerAction* action);
	int getNumScheduledTasks();

private:
	void run() override;
	void onFinished(int threadID) override;

	int batchSize = 5;
	int batchDelay = 1.0f;
	float ticks = 0.0f;

	String name;
	bool running = false;
	int  numWorkers = 0;
	ActiveThreadMap activeThreads;
	ThreadList inactiveThreads;
	ActionList pendingActions;

};

