#include "Cube.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "CameraManager.h"
#include "EngineTime.h"
#include "ShaderLibrary.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObjectManager.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "TextureComponent.h"

using namespace GDEngine;

Cube::Cube(std::string name) : AGameObject(name)
{
	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
	Vector3D positionList[] =
	{
		Vector3D(-1.0f,-1.0f,-1.0f),
		Vector3D(-1.0f,1.0f, -1.0f),
		Vector3D(1.0f, 1.0f, -1.0f),
		Vector3D(1.0f, -1.0f,-1.0f),

		Vector3D(1.0f, -1.0f,1.0f),
		Vector3D(1.0f, 1.0f, 1.0f),
		Vector3D(-1.0f,1.0f, 1.0f),
		Vector3D(-1.0f,-1.0f,1.0f)
	};

	Vector2D texCoordList[] =
	{
		Vector2D(0.0f, 0.0f),
		Vector2D(0.0f, 2.0f),
		Vector2D(2.0f, 0.0f),
		Vector2D(2.0f, 2.0f),
	};

	texVertex vertexList[] =
	{
		{positionList[0], texCoordList[1]},
		{positionList[1], texCoordList[0]},
		{positionList[2], texCoordList[2]},
		{positionList[3], texCoordList[3]},

		{positionList[4], texCoordList[1]},
		{positionList[5], texCoordList[0]},
		{positionList[6], texCoordList[2]},
		{positionList[7], texCoordList[3]},

		{positionList[1], texCoordList[1]},
		{positionList[6], texCoordList[0]},
		{positionList[5], texCoordList[2]},
		{positionList[2], texCoordList[3]},

		{positionList[7], texCoordList[1]},
		{positionList[0], texCoordList[0]},
		{positionList[3], texCoordList[2]},
		{positionList[4], texCoordList[3]},

		{positionList[3], texCoordList[1]},
		{positionList[2], texCoordList[0]},
		{positionList[5], texCoordList[2]},
		{positionList[4], texCoordList[3]},

		{positionList[7], texCoordList[1]},
		{positionList[6], texCoordList[0]},
		{positionList[1], texCoordList[2]},
		{positionList[0], texCoordList[3]},
	};

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	ShaderNames shaderNames;

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertexList, sizeof(texVertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20
	};

	m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	CBObjectData cbData;
	angle = 0.0f;
	cbData.time = angle;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

	speed = 1.0f;
	deltaRotation = 0.0f;
}

Cube::Cube(std::string guid, std::string name) : AGameObject(guid, name)
{
	Vector3D positionList[] =
	{
		Vector3D(-1.0f,-1.0f,-1.0f),
		Vector3D(-1.0f,1.0f, -1.0f),
		Vector3D(1.0f, 1.0f, -1.0f),
		Vector3D(1.0f, -1.0f,-1.0f),

		Vector3D(1.0f, -1.0f,1.0f),
		Vector3D(1.0f, 1.0f, 1.0f),
		Vector3D(-1.0f,1.0f, 1.0f),
		Vector3D(-1.0f,-1.0f,1.0f)
	};

	Vector2D texCoordList[] =
	{
		Vector2D(0.0f, 0.0f),
		Vector2D(0.0f, 2.0f),
		Vector2D(2.0f, 0.0f),
		Vector2D(2.0f, 2.0f),
	};

	texVertex vertexList[] =
	{
		{positionList[0], texCoordList[1]},
		{positionList[1], texCoordList[0]},
		{positionList[2], texCoordList[2]},
		{positionList[3], texCoordList[3]},

		{positionList[4], texCoordList[1]},
		{positionList[5], texCoordList[0]},
		{positionList[6], texCoordList[2]},
		{positionList[7], texCoordList[3]},

		{positionList[1], texCoordList[1]},
		{positionList[6], texCoordList[0]},
		{positionList[5], texCoordList[2]},
		{positionList[2], texCoordList[3]},

		{positionList[7], texCoordList[1]},
		{positionList[0], texCoordList[0]},
		{positionList[3], texCoordList[2]},
		{positionList[4], texCoordList[3]},

		{positionList[3], texCoordList[1]},
		{positionList[2], texCoordList[0]},
		{positionList[5], texCoordList[2]},
		{positionList[4], texCoordList[3]},

		{positionList[7], texCoordList[1]},
		{positionList[6], texCoordList[0]},
		{positionList[1], texCoordList[2]},
		{positionList[0], texCoordList[3]},
	};

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	ShaderNames shaderNames;

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	Logger::log("Trying to create Cube Textured Vertex Buffer");
	m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertexList, sizeof(texVertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20
	};

	Logger::log("Trying to create Cube Index Buffer");
	m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	CBObjectData cbData;
	angle = 0.0f;
	cbData.time = angle;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

	speed = 1.0f;
	deltaRotation = 0.0f;
}

Cube::~Cube()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_constantBuffer;
	AGameObject::~AGameObject();
}

void Cube::onCreate()
{
	AGameObject::onCreate();
}

void Cube::update(float deltaTime)
{
	AGameObject::update(deltaTime);
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	CBObjectData cbObjectData;

	cbObjectData.time = 0.0f;

	elapsedTime += EngineTime::getDeltaTime() / 2.0f;

	this->updateLocalMatrix();

	cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

	m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
}

// Sets shaders and draws afterwards
void Cube::draw(int height, int width)
{
	ShaderNames shaderNames;
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
	pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);

	if (this->texture != nullptr)
		renderSystem->getImmediateDeviceContext()->setTexture(this->texture);

	renderSystem->getImmediateDeviceContext()->setConstantBuffer(m_constantBuffer, 0);

	std::vector<AComponent*> rendererList = this->getComponentsOfType(AComponent::Renderer);
	if (!rendererList.empty())
	{
		ARenderer* renderer;
		for (AComponent* component : rendererList) {
			renderer = (ARenderer*)component;
			renderer->perform(EngineTime::getDeltaTime());
		}

		rendererList = this->getComponentsOfTypeRecursive(AComponent::Renderer);

		for (AComponent* component : rendererList) {
			renderer = (ARenderer*)component;
			renderer->perform(EngineTime::getDeltaTime());
		}
		return;
	}

	renderSystem->getImmediateDeviceContext()->setVertexShader(vertexShader);
	renderSystem->getImmediateDeviceContext()->setPixelShader(pixelShader);

	renderSystem->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	renderSystem->getImmediateDeviceContext()->setIndexBuffer(m_indexBuffer);

	renderSystem->getImmediateDeviceContext()->drawIndexedTriangleList(m_indexBuffer->getSizeIndexList(),0,0);
}

void Cube::onDestroy()
{
	
}

void Cube::setSpeed(float speed)
{
	this->speed = speed;
}

void GDEngine::Cube::toggleTexture()
{
	this->m_vertexBuffer = NULL;
	this->m_indexBuffer = NULL;

	if (this->m_texture)
	{
		Vector3D positionList[] =
		{
			Vector3D(-1.0f,-1.0f,-1.0f),
			Vector3D(-1.0f, 1.0f, -1.0f),
			Vector3D(1.0f, 1.0f, -1.0f),
			Vector3D(1.0f, -1.0f,-1.0f),

			Vector3D(1.0f, -1.0f,1.0f),
			Vector3D(1.0f, 1.0f, 1.0f),
			Vector3D(-1.0f, 1.0f, 1.0f),
			Vector3D(-1.0f,-1.0f,1.0f)
		};

		Vector2D texCoordList[] =
		{
			Vector2D(0.0f, 0.0f),
			Vector2D(0.0f, 1.0f),
			Vector2D(1.0f, 0.0f),
			Vector2D(1.0f, 1.0f),
		};

		texVertex vertexList[] =
		{
			{positionList[0], texCoordList[1]},
			{positionList[1], texCoordList[0]},
			{positionList[2], texCoordList[2]},
			{positionList[3], texCoordList[3]},

			{positionList[4], texCoordList[1]},
			{positionList[5], texCoordList[0]},
			{positionList[6], texCoordList[2]},
			{positionList[7], texCoordList[3]},

			{positionList[1], texCoordList[1]},
			{positionList[6], texCoordList[0]},
			{positionList[5], texCoordList[2]},
			{positionList[2], texCoordList[3]},

			{positionList[7], texCoordList[1]},
			{positionList[0], texCoordList[0]},
			{positionList[3], texCoordList[2]},
			{positionList[4], texCoordList[3]},

			{positionList[3], texCoordList[1]},
			{positionList[2], texCoordList[0]},
			{positionList[5], texCoordList[2]},
			{positionList[4], texCoordList[3]},

			{positionList[7], texCoordList[1]},
			{positionList[6], texCoordList[0]},
			{positionList[1], texCoordList[2]},
			{positionList[0], texCoordList[3]},
		};

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		ShaderNames shaderNames;

		void* shaderByteCode = nullptr;
		size_t sizeShader = 0;

		ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

		m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertexList, sizeof(vertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

		unsigned int indexList[] = {
			0,1,2,
			2,3,0,

			4,5,6,
			6,7,4,

			1,6,5,
			5,2,1,

			7,0,3,
			3,4,7,

			3,2,5,
			5,4,3,

			7,6,1,
			1,0,7
		};

		m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));
	}
	else
	{
		Vector3D color1 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
		Vector3D color2 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
		Vector3D color3 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
		Vector3D color4 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

		Vector3D color5 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
		Vector3D color6 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
		Vector3D color7 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
		Vector3D color8 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

		vertex vertexList[] =
		{
			{Vector3D(-1.0f,-1.0f,-1.0f),color3, color7 },
			{Vector3D(-1.0f,1.0f, -1.0f),color1, color5 },
			{Vector3D(1.0f, 1.0f, -1.0f),color1, color5 },
			{Vector3D(1.0f, -1.0f,-1.0f),color3, color7 },

			{Vector3D(1.0f, -1.0f,1.0f), color4, color8 },
			{Vector3D(1.0f, 1.0f, 1.0f), color2, color6 },
			{Vector3D(-1.0f,1.0f, 1.0f), color2, color6 },
			{Vector3D(-1.0f,-1.0f,1.0f), color4, color8 }
		};

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		ShaderNames shaderNames;

		void* shaderByteCode = NULL;
		size_t sizeShader = 0;

		ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

		m_vertexBuffer = renderSystem->createVertexBuffer(vertexList, sizeof(vertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

		unsigned int indexList[] = {
			0,1,2,
			2,3,0,

			4,5,6,
			6,7,4,

			1,6,5,
			5,2,1,

			7,0,3,
			3,4,7,

			3,2,5,
			5,4,3,

			7,6,1,
			1,0,7
		};

		m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));
	}
}
