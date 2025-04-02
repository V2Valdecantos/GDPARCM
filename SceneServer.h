#pragma once

#define TINYOBJLOADER_IMPLEMENTATION

#include <cstdint>
#include <vector>

#include "scenestreamer.pb.h"
#include "scenestreamer.grpc.pb.h"
#include "IETThread.h"

struct Pos 
{
	int x, y, z;
};

struct Scale 
{
	int x, y, z;
};

typedef std::vector<Scene*> SceneList;

class SceneStreamerServer final : public SceneStreamer::Service, public IETThread
{
	public:
		grpc::Status RequestScene(grpc::ServerContext* context, const SceneIndex* request, grpc::ServerWriter<Scene>* writer) override;

		std::byte* loadBytesFromFile(const wchar_t* path);
		void initializeScenes();
		
		void run() override;

		static void RunServer(uint16_t port);

		bool isRunning = false;

	
	private:
		SceneList sceneList;
		Scene* response;
};

