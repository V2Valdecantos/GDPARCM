#pragma once

#define TINYOBJLOADER_IMPLEMENTATION

#include <cstdint>
#include <vector>

#include "scenestreamer.pb.h"
#include "scenestreamer.grpc.pb.h"
#include "IETThread.h"

#include "tiny_obj_loader.h"

struct Pos 
{
	int x, y, z;
};

struct Scale 
{
	int x, y, z;
};
struct Asset 
{
	Pos pos;
	Scale scale;
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

};

typedef std::vector<Asset> Scene;

class SceneStreamerServer final : public SceneStreamer::Service, public IETThread
{
	public:
		grpc::Status RequestScene(grpc::ServerContext* context, const SceneIndex* request, Scene* response) override;

		Asset loadAssetFromFile(std::string path);
		void initializeScenes();
		
		void run() override;

		static void RunServer(uint16_t port);

		bool isRunning = false;

	private:
		std::vector<Scene> sceneList;
};

