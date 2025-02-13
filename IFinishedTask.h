#pragma once
class IFinishedTask
{
public:
	virtual void onFinished(int threadID) = 0;
};

