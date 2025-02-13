#include "HelloWorldThread.h"
#include <iostream>
HelloWorldThread::HelloWorldThread(int id)
{
	this->id = id;
}

HelloWorldThread::~HelloWorldThread()
{
	
}

void HelloWorldThread::run()
{
	std::cout << "This is a hello world in a thread # " << this->id << "\n";
}


