#pragma once
#include "IETThread.h"
class IExecutionEvent;
class StreamAssetLoader :    public IETThread
{
private:
	typedef std::string String;
public:
	StreamAssetLoader(String path, IExecutionEvent* executionEvent);
	~StreamAssetLoader();

private:
	void run() override;

	String path;
	IExecutionEvent* execEvent;
};

