#include "GreeterClient.h"
#include <grpcpp/create_channel.h>

GreeterClient::GreeterClient(std::shared_ptr<grpc::ChannelInterface> channel)
{
    this->stub_ = Greeter::NewStub(channel);
}


std::string GreeterClient::SayHello(const std::string& user)
{
    // Follows the same pattern as SayHello.
    HelloRequest request;
    request.set_name(user);
    HelloReply reply;
    grpc::ClientContext context;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    // Here we can use the stub's newly available method we just added.
    grpc::Status status = stub_->SayHello(&context, request, &reply);
    if (status.ok()) {
        return reply.message();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";
    }
}

std::string GreeterClient::SayHelloAgain(const std::string& user)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Follows the same pattern as SayHello.
    HelloRequest request;
    request.set_name(user);
    HelloReply reply;
    grpc::ClientContext context;

    // Here we can use the stub's newly available method we just added.
    grpc::Status status = stub_->SayHelloAgain(&context, request, &reply);
    std::cout << "[CLIENT]: Sending " << this->name << " to server" << std::endl;
    if (status.ok()) {
        std::cout << "[CLIENT]: Success sending " << this->name << " to server" << std::endl;
        return reply.message();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";
    }

}

void GreeterClient::runClient(std::vector<std::string> names)
{
    std::vector<GreeterClient*> clients;
    bool isRunning = true;

    for (int i = 0; i < names.size(); i++) 
    {
        GreeterClient* greeter = new GreeterClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
        greeter->name = names[i];
        clients.push_back(greeter);
        std::string reply = greeter->SayHello(greeter->name);

        //std::cout << "[SERVER]: " << reply << std::endl;

        greeter->SayHelloAgain(greeter->name);
    }

    while (isRunning) {

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> delay(0, 1);
        std::this_thread::sleep_for(std::chrono::seconds(delay(rng)));


        for (int i = 0; i < clients.size(); i++) 
        {
            clients[i]->SayHelloAgain(clients[i]->name);
        }
    }

    //std::string user("Student #1");
    //std::string reply1 = greeter1.SayHello(user);

    //std::cout << "[GREETER]: " << reply1 << std::endl;
}
