#include "StreamingManager.h"
#include "EngineTime.h"
#include "GameObjectManager.h"
#include "MeshManager.h"
#include "MeshObject.h"

#include <fstream>

#include "Logger.h"

#include <grpcpp/create_channel.h>

namespace GDEngine {
	StreamingManager* StreamingManager::P_SHARED_INSTANCE = nullptr;

	void StreamingManager::startStreamingScene(int index)
	{
		//retry connection to server
		//this->stub_ = SceneStreamer::NewStub(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
		std::thread streamer(&StreamingManager::RequestScene, this, index);
		streamer.detach();
	}

	void StreamingManager::streamScene(int index)
	{
		//this->RequestScene(index);
	}

	void StreamingManager::RequestScene(int index)
	{
		
		SceneIndex request; 
		request.set_sceneid(index);
		//grpc::ClientContext pingcontext;
		grpc::ClientContext context;

		////"ping" the server
		//msg pingmsg; pingmsg.set_strmsg("Requesting Scene " + std::to_string(index));
		//msg reply;
		//grpc::Status status = this->stub_->Ping(&pingcontext, pingmsg, &reply);

		//if (status.ok())
		//	Logger::log(this, reply.strmsg());

		//scene requesting
		Scene response;
		std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(15000);
		context.set_deadline(deadline);

		// Here we can use the stub's newly available method we just added.
		std::unique_ptr<grpc::ClientReader<Scene>> reader(this->stub_->RequestScene(&context, request));
		this->sceneFlags[index] = true;


		while (reader->Read(&response))
		{
			Logger::log(this, "Starting stream");
			this->createMeshObjectFromStream(response.asset1(), index, 1);
			Logger::log(this, "Streamed Asset");
			this->createMeshObjectFromStream(response.asset2(), index, 2);
			Logger::log(this, "Streamed Asset");
		
		}

		Logger::log(this, "Done Streaming");
	}

	bool StreamingManager::createMeshObjectFromStream(std::string bytes, int sceneID, int index)
	{
		//load the bytes into a new obj file
		std::string modelLocation = "assets/streamed/"; \
		std::string modelName = "scene_" + std::to_string(sceneID) + "_object" + std::to_string(index);
		std::string filePath = modelLocation + modelName + ".obj";
		//convert path to wchar_t
		std::wstring widestr = std::wstring(filePath.begin(), filePath.end());
		const wchar_t* charPath = widestr.c_str();

		std::ofstream file(filePath, std::ios::binary);
		file << bytes;

		//create the gameobject
		MeshObject* object = new MeshObject(modelName, charPath);
		object->setScale(10,10,10);
		GameObjectManager::getInstance()->addObject(object);

		return true;
	}


	StreamingManager::StreamingManager(std::shared_ptr<grpc::ChannelInterface> channel)
	{
		this->stub_ = SceneStreamer::NewStub(channel);
	}

	StreamingManager::~StreamingManager()
	{
	}

	StreamingManager* StreamingManager::getInstance()
	{
		return P_SHARED_INSTANCE;
	}

	void StreamingManager::initialize(std::shared_ptr<grpc::ChannelInterface> channel)
	{
		if (P_SHARED_INSTANCE)
			Logger::throw_exception("Streaming Manager already created");
		P_SHARED_INSTANCE = new StreamingManager(channel);
	}

	void StreamingManager::destroy()
	{
	}
}
