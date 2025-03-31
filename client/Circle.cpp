#include "Circle.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "Random.h"
#include "ShaderLibrary.h"

using namespace GDEngine;

Circle::Circle(std::string name, float radius, int sides, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	this->radius = radius;
	this->sides = sides;
	m_vertexBuffer = nullptr;

	RECT windowRect = AppWindow::getInstance()->getClientWindowRect();

	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);	
	FLOAT width = windowRect.right - windowRect.left;
	FLOAT height = windowRect.bottom - windowRect.top;

	float orthoWidth = width / 300.0f;
	float orthoHeight = height / 300.0f;

	this->radius /= 300.0f;

	float deltaTheta = 2 * (atan(1) * 4) / sides; // Change in theta for each vertex
	for (int i = 0; i < sides; i++) {
		float theta = i * deltaTheta; // Theta is the angle for that triangle
		float index = 3 * i;
		list.push_back({ Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0,0,0), Vector3D(0,0,0) });
		list.push_back({ Vector3D(cos(theta) * radius, sin(theta) * radius, 0.0f), Vector3D(1,i % 2,1), Vector3D(0,0,1) });
		list.push_back({ Vector3D(cos(theta + deltaTheta) * radius, sin(theta + deltaTheta) * radius, 0.0f), Vector3D(1,i % 2,1), Vector3D(0,0,1) });
	}

	CBObjectData cc;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	m_constantBuffer = renderSystem->createConstantBuffer(&cc, sizeof(CBObjectData));

	UINT size_list = list.size();

	m_vertexBuffer = renderSystem->createVertexBuffer(list.data(), sizeof(vertex), size_list, shaderByteCode, sizeShader);
}

Circle::~Circle()
{
}

void Circle::onCreate()
{
	//Vector3D direction
	float xDirection = Random::range(-100, 100);
	float yDirection = Random::range(-100, 100);

	direction = Vector3D(xDirection, yDirection, 0.0f);
	direction = direction.normalize();
	speed = 1.0f;
}

void Circle::update(float deltaTime)
{
	CBObjectData cc;

	RECT windowRect = AppWindow::getInstance()->getClientWindowRect();

	FLOAT width = windowRect.right - windowRect.left;
	FLOAT height = windowRect.bottom - windowRect.top;

	float orthoWidth = width / 300.0f;
	float orthoHeight = height / 300.0f;

	if (m_localPosition.y + this->radius >= orthoHeight / 2)
	{
		direction.y = direction.y * -1.0f;
		m_localPosition.y = (orthoHeight / 2) - this->radius;
	}
		
	if (m_localPosition.y - this->radius <= -orthoHeight / 2)
	{
		direction.y = direction.y * -1.0f;
		m_localPosition.y = -(orthoHeight / 2) + this->radius;
	}

	if (m_localPosition.x + this->radius >= orthoWidth / 2)
	{
		direction.x = direction.x * -1.0f;
		m_localPosition.x = (orthoWidth / 2) - this->radius;
	}
		
	if (m_localPosition.x - this->radius <= -orthoWidth / 2)
	{
		direction.x = direction.x * -1.0f;
		m_localPosition.x = -(orthoWidth / 2) + this->radius;
	}
		

	m_localPosition += direction * speed * EngineTime::getDeltaTime();

	cc.worldMatrix.setTranslation(m_localPosition);

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cc);
}

// Sets shaders and draws afterwards
void Circle::draw(int width, int height)
{
	ShaderNames shaderNames;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
	pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);
	renderSystem->getImmediateDeviceContext()->setTexture(this->texture);


	renderSystem->getImmediateDeviceContext()->setConstantBuffer(m_constantBuffer, 0);

	renderSystem->getImmediateDeviceContext()->setVertexShader(vertexShader);
	renderSystem->getImmediateDeviceContext()->setPixelShader(pixelShader);

	renderSystem->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);

	renderSystem->getImmediateDeviceContext()->drawTriangleStrip(m_vertexBuffer->getSizeVertexList(), 0);
}

void Circle::onDestroy()
{
	delete m_vertexBuffer;
	delete m_constantBuffer;
	delete this;
}