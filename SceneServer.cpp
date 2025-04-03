#include "SceneServer.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <array>
#include <memory>
#include <type_traits>
#include <iostream>
#include <iomanip>

#include <fstream>

#include "GraphicsEngine.h"
#include "MeshObject.h"

using namespace GDEngine;

grpc::Status SceneStreamerServer::RequestScene(grpc::ServerContext* context, const SceneIndex* request, grpc::ServerWriter<Scene>* writer)
{
	std::cout << "[SERVER]: Sending Scene." << std::endl;
	Scene response;
	response.set_asset1(this->sceneList[request->sceneid()]->asset1());
	response.set_asset2(this->sceneList[request->sceneid()]->asset2());

	writer->Write(response);
	return grpc::Status::OK;
}

std::stringstream SceneStreamerServer::loadBytesFromFile(const wchar_t* path)
{	 

	const std::ifstream file(path, std::ios::binary);
	std::stringstream objBytes;
	objBytes << file.rdbuf();

	return objBytes;
}

void SceneStreamerServer::initializeScenes()
{

	GraphicsEngine::initialize();

	for (int i = 0; i < 4; i++) 
	{
		Scene* scene = new Scene();
		sceneList.push_back(scene);
	}
	
	//Scene 1
	std::stringstream bunny_bytes = this->loadBytesFromFile(L"assets/meshes/bunny.obj");
	std::stringstream lucy_bytes = this->loadBytesFromFile(L"assets/meshes/bunny.obj");
	this->sceneList[0]->set_asset1(bunny_bytes.str());
	this->sceneList[0]->set_asset2(lucy_bytes.str());


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
	std::cout << "< INITIALIZED SCENES > " << serverAddress << std::endl;

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	grpc::ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	builder.SetMaxMessageSize(INT_MAX);
	builder.SetMaxSendMessageSize(INT_MAX);
	builder.SetMaxReceiveMessageSize(INT_MAX);
	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << serverAddress << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}
