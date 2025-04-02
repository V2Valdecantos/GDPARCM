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
		grpc::ClientContext context;

		Scene response;

		std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(15000);
		context.set_deadline(deadline);

		// Here we can use the stub's newly available method we just added.
		std::unique_ptr<grpc::ClientReader<Scene>> reader(this->stub_->RequestScene(&context, request));
		this->sceneFlags[index] = true;


		while (reader->Read(&response))
		{
			this->createMeshObjectFromStream(response.asset1(), index, 1);
			Logger::log(this, "Streamed Asset");
			this->createMeshObjectFromStream(response.asset2(), index, 2);
			Logger::log(this, "Streamed Asset");
		
		}

		grpc::Status status = reader->Finish();
		Logger::log(this, "Done Streaming");

	}

	bool StreamingManager::createMeshObjectFromStream(std::string bytes, int sceneID, int index)
	{

		//make obj file
		std::string modelLoc = "assets/streamed/";
		std::string fileName = "scene_" + std::to_string(sceneID) + "_object_" + std::to_string(index);
		std::string path = modelLoc + fileName + ".obj";
		std::ofstream file(fileName, std::ios::binary);

		file << bytes;

		//filepath
		std::wstring widestr = std::wstring(path.begin(), path.end());
		const wchar_t* charPath = widestr.c_str();

		MeshObject* obj = new MeshObject(fileName, charPath);
		obj->setPosition(0, 0, 0);
		obj->setScale(10, 10, 10);

		GameObjectManager::getInstance()->addObject(obj);
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
