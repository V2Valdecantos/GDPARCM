// GDPARCM_HO8 - gRPC Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GreeterServer.h"
#include "GreeterClient.h"

int n = 10; //number of students
int c = 2; //number of clients
int cs = -1; //input
std::string input;

int main()
{

    std::cin >> cs;

    if (cs == 1) 
    {
        //run the server
        GreeterServer server;
        server.start();

        //wait for server to startup
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        while(server.isRunning) {} //wait for shutdown
    }
    else 
    {
        bool isRunning = true;
        std::vector<std::string> names;

        for (int i = 0; i < n / c; i++) 
        {
            std::cin >> input;
            names.push_back(input);
        }


        //run the client
        GreeterClient::runClient(names);

        while (isRunning) {} //wait for shutdown
    }


}
