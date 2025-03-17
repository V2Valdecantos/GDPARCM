#include <iostream>
#include "mutex"
#include "string"
#include "thread"


std::mutex guard;


void printMessage(int id, std::string message)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));

	guard.lock();
	std::cout << "Thread [" << id << "]: " << message << std::endl;
	guard.unlock();
}

int main() 
{
	////join
	//for (int i = 0; i < 12; i++) 
	//{
	//	std::thread t = std::thread(printMessage, i, "Join Finished Execution");
	//	t.join();
	//}

	//detach
	for (int i = 0; i < 12; i++)
	{
		std::thread t = std::thread(printMessage, i, " Detach Finished Execution");
		t.detach();
	}

	std::this_thread::sleep_for(std::chrono::seconds(20));
}

