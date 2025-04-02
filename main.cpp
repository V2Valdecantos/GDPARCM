#include "AppWindow.h"
#include "SceneServer.h"
#include "iostream"

/*
MODIFIED FOR GDPARCM

THIS PROJECT SERVES AS THE CLIENT RENDERING SYSTEM
*/
int main()
{
	

	AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
	//runningApp->initializeEngine();

	int input = -1;
	do
	{
		std::cout << "[SYSYEM]: Enter 1 for Server; 2 for Client." << std::endl;
		std::cout << "[INPUT]: ";
		std::cin >> input;

		if (input == 1 || input == 2)
			break;

	} while (input != 1 || input != 2);

	if (input == 2) 
	{

		while (runningApp->isRunning())
		{
			runningApp->broadcast();
		}
	}
	else
	{
		SceneStreamerServer server;
		server.run();

	}
	return 0;
}