#include "AppWindow.h"


/*
MODIFIED FOR GDPARCM

THIS PROJECT SERVES AS THE CLIENT RENDERING SYSTEM
*/
int main()
{
	AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
	//runningApp->initializeEngine();

	while(runningApp->isRunning())
	{
		runningApp->broadcast();
	}

	return 0;
}