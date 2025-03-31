#include "SceneServer.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


grpc::Status SceneStreamerServer::RequestScene(grpc::ServerContext* context, const SceneIndex* request, Scene* response)
{

	for (Asset asset : this->sceneList[request->sceneid()]) 
	{
		response->push_back(asset);
	}
	
	return grpc::Status::OK;
}

Asset SceneStreamerServer::loadAssetFromFile(std::string path)
{
	std::string inputFile = std::filesystem::path(path).string();
	std::string warn;
	std::string err;
	Asset loadedAsset;
	bool result = tinyobj::LoadObj(&loadedAsset.attribs, &loadedAsset.shapes, &loadedAsset.materials, &warn, &err, inputFile.c_str());
	loadedAsset.pos = { 0,0,0 };
	loadedAsset.scale = { 1,1,1 };

	return loadedAsset;
}

void SceneStreamerServer::initializeScenes()
{
	//Scene 1
	std::vector<Asset> scene1;
	Asset bunny = this->loadAssetFromFile("assets/models/bunny.obj");
	bunny.pos = { 0,0,0 };
	bunny.scale = { 10,10,10 };
	scene1.push_back(bunny);

	this->sceneList.push_back(scene1);


	//Scene 2
}

void SceneStreamerServer::run()
{
	this->isRunning = true;
	this->RunServer(50051);
}

void SceneStreamerServer::RunServer(uint16_t port)
{
	std::string serverAddress = absl::StrFormat("localhost:%d", port);
	SceneStreamerServer service;

	//initialize all scenes to be streamed
	service.initializeScenes();

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	grpc::ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << serverAddress << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}
