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
	std::cout << "[SERVER]: Sending Scene " + std::to_string(request->sceneid() + 1) << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3)); //simulate network latency
	Scene response;
	response.set_asset1(this->sceneList[request->sceneid()]->asset1());
	response.set_asset2(this->sceneList[request->sceneid()]->asset2());
	response.set_asset3(this->sceneList[request->sceneid()]->asset3());
	response.set_asset4(this->sceneList[request->sceneid()]->asset4());
	response.set_asset5(this->sceneList[request->sceneid()]->asset5());

	writer->Write(response);

	return grpc::Status::OK;
}

std::stringstream SceneStreamerServer::loadBytesFromFile(const wchar_t* path)
{	 

	std::ifstream file(path, std::ios::binary);
	std::stringstream objBytes;

	while (file) 
	{
		objBytes << file.rdbuf();

		if (objBytes.gcount() >= file.gcount())
			file.close();
	}

	return objBytes;
}

void SceneStreamerServer::initializeScenes()
{

	GraphicsEngine::initialize();

	for (int i = 0; i < 5; i++) 
	{
		Scene* scene = new Scene();
		sceneList.push_back(scene);
	}
	
	//Scene 1
	std::stringstream bunny_bytes = this->loadBytesFromFile(L"assets/meshes/bunny.obj");
	std::stringstream tea_bytes = this->loadBytesFromFile(L"assets/meshes/teapot.obj");
	std::stringstream cat_bytes = this->loadBytesFromFile(L"assets/meshes/cat.obj");
	std::stringstream tree_bytes = this->loadBytesFromFile(L"assets/meshes/tree.obj");
	std::stringstream fish_bytes = this->loadBytesFromFile(L"assets/meshes/fish.obj");
	this->sceneList[0]->set_asset1(bunny_bytes.str());
	this->sceneList[0]->set_asset2(tea_bytes.str());
	this->sceneList[0]->set_asset3(cat_bytes.str());
	this->sceneList[0]->set_asset4(tree_bytes.str());
	this->sceneList[0]->set_asset5(fish_bytes.str());


	//Scene 2
	std::stringstream guy_bytes = this->loadBytesFromFile(L"assets/meshes/guy.obj");
	std::stringstream lamp_bytes = this->loadBytesFromFile(L"assets/meshes/bunny.obj");
	std::stringstream bird_bytes = this->loadBytesFromFile(L"assets/meshes/bird.obj");
	std::stringstream dolphin_bytes = this->loadBytesFromFile(L"assets/meshes/dolphin.obj");
	std::stringstream whale_bytes = this->loadBytesFromFile(L"assets/meshes/whale.obj");
	this->sceneList[1]->set_asset1(guy_bytes.str());
	this->sceneList[1]->set_asset2(lamp_bytes.str());
	this->sceneList[1]->set_asset3(bird_bytes.str());
	this->sceneList[1]->set_asset4(dolphin_bytes.str());
	this->sceneList[1]->set_asset5(whale_bytes.str());

	//Scene 3
	std::stringstream s31_bytes = this->loadBytesFromFile(L"assets/meshes/car.obj");
	std::stringstream s32_bytes = this->loadBytesFromFile(L"assets/meshes/lucy.obj");
	std::stringstream s33_bytes = this->loadBytesFromFile(L"assets/meshes/armadillo.obj");
	std::stringstream s34_bytes = this->loadBytesFromFile(L"assets/meshes/dog.obj");
	std::stringstream s35_bytes = this->loadBytesFromFile(L"assets/meshes/tree.obj");
	this->sceneList[2]->set_asset1(s31_bytes.str());
	this->sceneList[2]->set_asset2(s32_bytes.str());
	this->sceneList[2]->set_asset3(s33_bytes.str());
	this->sceneList[2]->set_asset4(s34_bytes.str());
	this->sceneList[2]->set_asset5(s35_bytes.str());

	//Scene 4
	std::stringstream s41_bytes = this->loadBytesFromFile(L"assets/meshes/deer.obj");
	std::stringstream s42_bytes = this->loadBytesFromFile(L"assets/meshes/plant.obj");
	std::stringstream s43_bytes = this->loadBytesFromFile(L"assets/meshes/toilet.obj");
	std::stringstream s44_bytes = this->loadBytesFromFile(L"assets/meshes/teapot.obj");
	std::stringstream s45_bytes = this->loadBytesFromFile(L"assets/meshes/guy.obj");
	this->sceneList[3]->set_asset1(s41_bytes.str());
	this->sceneList[3]->set_asset2(s42_bytes.str()); 
	this->sceneList[3]->set_asset3(s43_bytes.str());
	this->sceneList[3]->set_asset4(s44_bytes.str());
	this->sceneList[3]->set_asset5(s45_bytes.str());

	//Scene 5
	std::stringstream s51_bytes = this->loadBytesFromFile(L"assets/meshes/window.obj");
	std::stringstream s52_bytes = this->loadBytesFromFile(L"assets/meshes/statue.obj");
	std::stringstream s53_bytes = this->loadBytesFromFile(L"assets/meshes/lucy.obj");
	std::stringstream s54_bytes = this->loadBytesFromFile(L"assets/meshes/car.obj");
	std::stringstream s55_bytes = this->loadBytesFromFile(L"assets/meshes/house.obj");
	this->sceneList[4]->set_asset1(s51_bytes.str());
	this->sceneList[4]->set_asset2(s52_bytes.str());
	this->sceneList[4]->set_asset3(s53_bytes.str());
	this->sceneList[4]->set_asset4(s54_bytes.str());
	this->sceneList[4]->set_asset5(s55_bytes.str());
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
	builder.SetMaxSendMessageSize(INT_MAX);
	builder.SetMaxReceiveMessageSize(INT_MAX);
	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << serverAddress << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}
