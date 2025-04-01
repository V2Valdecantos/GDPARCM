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


#include "GraphicsEngine.h"
#include "MeshObject.h"

using namespace GDEngine;

grpc::Status SceneStreamerServer::RequestScene(grpc::ServerContext* context, const SceneIndex* request, grpc::ServerWriter<Scene>* writer)
{
	Scene response;
	response.set_asset1(this->sceneList[request->sceneid()]->asset1());
	response.set_asset2(this->sceneList[request->sceneid()]->asset2());

	writer->Write(response);
	return grpc::Status::OK;
}

std::string SceneStreamerServer::loadBytesFromFile(const wchar_t* path)
{	 

	//make a mesh object
	MeshObject* mesh = new MeshObject("obj", path);
	MeshObject* obj;

	//convert it to a char array
	std::array<byte, sizeof(MeshObject) > bytes;
	const byte* begin = reinterpret_cast<const byte*>(std::addressof(mesh));
	const byte* end = begin + sizeof(MeshObject);
	std::copy(begin, end, std::begin(bytes));
 
	//assign it to the return value
	std::string returnBytes(bytes.begin(), bytes.end());

	//test mesh
	//byte* begin_object = reinterpret_cast<byte*>(std::addressof(obj));
	//std::copy(std::begin(returnBytes), std::end(returnBytes), begin_object);

	std::cout << "Converted mesh to bytes: " << std::endl << returnBytes << std::endl;
	return returnBytes;
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
	std::string bunny_bytes = this->loadBytesFromFile(L"assets/meshes/bunny.obj");
	std::string lucy_bytes = this->loadBytesFromFile(L"assets/meshes/lucy.obj");
	this->sceneList[0]->set_asset1(bunny_bytes);
	this->sceneList[0]->set_asset2(lucy_bytes);


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
	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << serverAddress << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}
