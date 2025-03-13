#pragma once
#include "IETThread.h"

class IWorkerThread;
class IFinishedTask;
class PoolWorkerThread : public IETThread
{
public:
	PoolWorkerThread(int threadID, IFinishedTask* finishedTask);
	~PoolWorkerThread();

	int getThreadID();
	void assignTask(IWorkerAction* action);

	void run() override;

private:
	int id = 0;
	IWorkerAction* action;
	IFinishedTask* finishedTask;
};

