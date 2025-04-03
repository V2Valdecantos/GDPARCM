#include "StreamingManager.h"
#include "EngineTime.h"
#include "GameObjectManager.h"
#include "MeshManager.h"
#include "MeshObject.h"

#include <filesystem>
#include <fstream>

#include "Logger.h"
#include "Random.h"

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
		std::stringstream a1, a2, a3, a4, a5;

		while (reader->Read(&response))
		{
			a1 << response.asset1();
			a2 << response.asset2();
			a3 << response.asset3();
			a4 << response.asset4();
			a5 << response.asset5();
		}
		grpc::Status status = reader->Finish();

		if (status.ok()) 
		{
			if (a1.str().length() > 1) 
			{
				this->createMeshObjectFromStream(a1.str(), index, 1);
				Logger::log(this, "Streamed Asset");
			}
			if (a2.str().length() > 1) {
				this->createMeshObjectFromStream(a2.str(), index, 2);
				Logger::log(this, "Streamed Asset");
			}
			if (a3.str().length() > 1)
			{
				this->createMeshObjectFromStream(a3.str(), index, 3);
				Logger::log(this, "Streamed Asset");
			}
			if (a4.str().length() > 1) {
				this->createMeshObjectFromStream(a4.str(), index, 4);
				Logger::log(this, "Streamed Asset");
			}
			if (a5.str().length() > 1)
			{
				this->createMeshObjectFromStream(a5.str(), index, 5);
				Logger::log(this, "Streamed Asset");
			}

			Logger::log(this, "Done Streaming");
		}
		else 
		{
			Logger::log(this, "Stream Failed");
			this->sceneFlags[index] = false;
		}
	}

	void StreamingManager::RemoveScene(int index)
	{
		this->resetProgress(index);
		guard.lock();
		for (std::string path : this->scenePaths[index])
		{
			std::filesystem::remove(path);
		}
		for (std::string name : this->sceneNames[index])
		{
			GameObjectManager::getInstance()->deleteObjectByName(name);
		}
		this->scenePaths[index].clear();
		this->sceneNames[index].clear();
		guard.unlock();

	}

	bool StreamingManager::createMeshObjectFromStream(std::string bytes, int sceneID, int index)
	{
		guard.lock();
		//make obj file
		std::string modelLoc = "assets/streamed/";
		std::string fileName = "scene_" + std::to_string(sceneID) + "_object_" + std::to_string(index);
		std::string path = modelLoc + fileName + ".obj";
		std::ofstream file(path, std::ios::binary);

		file << bytes;
		

		//filepath
		std::wstring widestr = std::wstring(path.begin(), path.end());
		const wchar_t* charPath = widestr.c_str();

		this->scenePaths[sceneID].push_back(path);
		this->sceneNames[sceneID].push_back(fileName);

		MeshObject* obj = new MeshObject(fileName, charPath);

		//randomize transforms
		float posx = Random::range(-1000, 1000);
		float posy = Random::range(-1000, 1000);
		float posz = Random::range(-1000, 1000);

		float rotx = Random::range(-90, 90);
		float roty = Random::range(-90, 90);
		float rotz = Random::range(-90, 90);

		obj->setRotation(rotx, roty, rotz);
		obj->setPosition(posx, posy, posz);
		obj->setScale(1, 1, 1);

		GameObjectManager::getInstance()->addObject(obj);
		guard.unlock();

		this->incrementProgress(sceneID);
		return true;
	}

	void StreamingManager::incrementProgress(int index)
	{
		progressGuard.lock();
		this->sceneProgress[index] += 0.2f;
		this->allScenesProgress += 1.0f / 25.0f;
		progressGuard.unlock();
	}

	void StreamingManager::resetProgress(int index)
	{
		progressGuard.lock();
		this->sceneProgress[index] = 0;
		this->allScenesProgress -= (1.0f / 25.0f) * 5;
		progressGuard.unlock();
	}

	float StreamingManager::getProgress(int index)
	{

		return this->sceneProgress[index];

	}

	float StreamingManager::getAllProgress()
	{
		return this->allScenesProgress;
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
