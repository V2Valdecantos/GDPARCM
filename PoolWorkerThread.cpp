#include "PoolWorkerThread.h"
#include "IWorkerAction.h"
#include "ThreadPool.h"

PoolWorkerThread::PoolWorkerThread(int threadID, IFinishedTask* finishedTask)
{
	this->id = id;
	this->finishedTask = finishedTask;
}

PoolWorkerThread::~PoolWorkerThread()
{
}

int PoolWorkerThread::getThreadID()
{
	return this->id;
}

void PoolWorkerThread::assignTask(IWorkerAction* action)
{
	this->action = action;
}

void PoolWorkerThread::run()
{
	this->action->onStartTask();
	this->finishedTask->onFinished(this->id);
}
