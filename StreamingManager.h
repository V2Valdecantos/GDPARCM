#pragma once
#include "IETThread.h"
#include <thread>
#include <mutex>
#include <unordered_map>

#include "scenestreamer.grpc.pb.h"
#include "iostream"

class MeshObject;
namespace GDEngine {
	class StreamingManager
	{

	public:
		void startStreamingScene(int index);
		void streamScene(int index);

		void RequestScene(int index);
		void RemoveScene(int index);
		bool sceneFlags[5] = { false, false, false, false, false};
		float sceneProgress[5] = { 0,0,0,0,0 };
		float allScenesProgress = 0.0f;
		float getProgress(int index);
		float getAllProgress();

	private:
		bool createMeshObjectFromStream(std::string bytes, int sceneID, int index);
		void incrementProgress(int index);
		void resetProgress(int index);

	private:
		static StreamingManager* P_SHARED_INSTANCE;
	private:
		StreamingManager(std::shared_ptr<grpc::ChannelInterface> channel);
		~StreamingManager();
		StreamingManager(const StreamingManager&);
		StreamingManager& operator = (const StreamingManager&);

	public:
		static StreamingManager* getInstance();
		static void initialize(std::shared_ptr<grpc::ChannelInterface> channel);
		static void destroy();

	private:
		std::unique_ptr<SceneStreamer::Stub> stub_;
		std::mutex guard;
		std::mutex progressGuard;
		std::unordered_map<int, std::vector<std::string>> scenePaths;
		std::unordered_map<int, std::vector<std::string>> sceneNames;
	};
}

