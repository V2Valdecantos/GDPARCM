#include "SceneServer.h"

int main()
{

    //run the server
    SceneStreamerServer server;
    server.start();

    //wait for server to startup
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while (server.isRunning) {} //wait for shutdown
  

}