#include "ThreadPool.h"
#include "PoolWorkerThread.h"
#include <SFML/Graphics.hpp>
ThreadPool::ThreadPool(String name, int numWorkers)
{
	this->name = name;
	this->numWorkers = numWorkers;

	for (int i = 0; i < this->numWorkers; i++) 
	{
		this->inactiveThreads.push(new PoolWorkerThread(i, this));
	}
}

ThreadPool::~ThreadPool()
{
	this->stopScheduler();
	this->activeThreads.clear();
	while (this->inactiveThreads.empty())
	{
		this->inactiveThreads.pop();
	}
}

void ThreadPool::startScheduler()
{
	this->running = true;
	this->start();
}

void ThreadPool::stopScheduler()
{
	this->running = false;
}

void ThreadPool::scheduleTask(IWorkerAction* action)
{
	this->pendingActions.push(action);
}

int ThreadPool::getNumScheduledTasks()
{
	return this->pendingActions.size();
}

void ThreadPool::run()
{
	//std::this_thread::sleep_for(std::chrono::seconds(10));
	while (this->running)
	{

		if (this->pendingActions.empty() == false)
		{
			if (this->inactiveThreads.empty() == false)
			{
				PoolWorkerThread* workerThread = this->inactiveThreads.front();
				this->inactiveThreads.pop();
				this->activeThreads[workerThread->getThreadID()] = workerThread;

				workerThread->assignTask(this->pendingActions.front());
				workerThread->start();
				this->pendingActions.pop();
			}

		}
	}

}

void ThreadPool::onFinished(int threadID)
{
	if (this->activeThreads[threadID] != nullptr)
	{
		//delete this->activeThreads[threadID];
		this->activeThreads.erase(threadID);

		this->inactiveThreads.push(new PoolWorkerThread(threadID, this));
	}

}
