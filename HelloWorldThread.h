#pragma once
#include "IETThread.h"
class HelloWorldThread: public IETThread

{
public:
	HelloWorldThread(int id);
	~HelloWorldThread();
	
private:
	void run() override;

	int id = 0;
};

