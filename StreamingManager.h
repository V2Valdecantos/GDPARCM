#pragma once
#include "IETThread.h"
#include <thread>
#include <mutex>

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

		bool sceneFlags[4] = { false, false, false, false };

	private:
		bool createMeshObjectFromStream(std::string bytes, int sceneID, int index);

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
	};
}

