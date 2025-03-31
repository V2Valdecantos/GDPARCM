#include "AppWindow.h"

#include <Windows.h>

#include "ActionHistory.h"
#include "imgui.h"

#include "Random.h"
#include "EngineTime.h"

#include "GraphicsEngine.h"

#include "BaseComponentSystem.h"
#include "PhysicsSystem.h"

#include "InputSystem.h"

#include "GameObjectManager.h"
#include "CameraManager.h"

#include "UIManager.h"
#include "ViewportManager.h"

#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "EngineBackend.h"
#include "ShaderLibrary.h"

#include "Logger.h"

using namespace GDEngine;

__declspec(align(16))
struct CBEditor
{
	int32_t wireframe;
};

void AppWindow::onCreate()
{
	Window::onCreate();
	InputSystem::initialize();
	initializeEngine();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	float deltaTime = EngineTime::getDeltaTime();

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	RECT windowRect = this->getClientWindowRect();

	FLOAT width = windowRect.right - windowRect.left;
	FLOAT height = windowRect.bottom - windowRect.top;

	renderSystem->getImmediateDeviceContext()->setViewportSize(width, height);

	EngineBackend* backend = EngineBackend::getInstance();
	if (backend->getMode() == EngineBackend::EditorMode::PLAY)
	{
		GameObjectManager::getInstance()->setPhysics(true);
		GameObjectManager::getInstance()->update(deltaTime);
		BaseComponentSystem::getInstance()->getPhysicsSystem()->updateAllComponents();
	}
	else if (backend->getMode() == EngineBackend::EditorMode::EDITOR)
	{
		GameObjectManager::getInstance()->setPhysics(false);
		GameObjectManager::getInstance()->update(deltaTime);

	}
	else if (backend->getMode() == EngineBackend::EditorMode::PAUSED)
	{
		if (backend->insideFrameStep())
		{
			GameObjectManager::getInstance()->update(deltaTime);
			BaseComponentSystem::getInstance()->getPhysicsSystem()->updateAllComponents();
			backend->endFrameStep();
		}
	}
	
	

	UIManager::getInstance()->draw();

	if (UIManager::RESIZE_WIDTH != 0 && UIManager::RESIZE_HEIGHT != 0)
	{
		swapChain->cleanRenderTarget();
		swapChain->resizeBuffers(0, UIManager::RESIZE_WIDTH, UIManager::RESIZE_HEIGHT);
		UIManager::RESIZE_WIDTH = UIManager::RESIZE_HEIGHT = 0;
		swapChain->createRenderTarget();
	}

	InputSystem::getInstance()->update();
	swapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	InputSystem::getInstance()->removeListener(this);

	GameObjectManager::getInstance()->deleteAllObjects();

	delete swapChain;

	UIManager::destroy();
	CameraManager::destroy();
	BaseComponentSystem::destroy();
	GameObjectManager::destroy();
	ViewportManager::destroy();
	ActionHistory::destroy();
	EngineBackend::destroy();
	ShaderLibrary::destroy();
	GraphicsEngine::destroy();
	InputSystem::destroy();
}

void AppWindow::onFocus()
{
	Window::onFocus();
	//InputSystem::getInstance()->startUpdate();
}

void AppWindow::onKillFocus()
{
	Window::onKillFocus();
	//InputSystem::getInstance()->stopUpdate();
}

void AppWindow::onKeyDown(int key)
{
	if (key == VK_SHIFT)
	{
		// Multiple Selection 
		GameObjectManager::getInstance()->setMultiselectMode(true);
	}
}

void AppWindow::onKeyUp(int key)
{
	if (key == 90) 
	{
		if (ActionHistory::getInstance()->hasRemainingUndoActions()) 
		{
			GameObjectManager::getInstance()->applyAction(ActionHistory::getInstance()->undoAction());
		}
	}

	if (key == 89) 
	{
		if (ActionHistory::getInstance()->hasRemainingRedoActions())
		{
			GameObjectManager::getInstance()->applyAction(ActionHistory::getInstance()->redoAction());
		}
	}

	if (key == VK_SHIFT)
	{
		// Multiple Selection 
		GameObjectManager::getInstance()->setMultiselectMode(false);
	}
}

void AppWindow::onMouseMove(const Vector2D& deltaMousePosition)
{
}

void AppWindow::onLeftMouseDown(const Vector2D& mousePosition)
{
}

void AppWindow::onLeftMouseUp(const Vector2D& mousePosition)
{
}

void AppWindow::onRightMouseDown(const Vector2D& mousePosition)
{
}

void AppWindow::onRightMouseUp(const Vector2D& mousePosition)
{
}

void AppWindow::initializeEngine()
{
	// Try Initializing Managers
	try
	{
		GraphicsEngine::initialize();
		ShaderLibrary::initialize();
		EngineBackend::initialize();
		ActionHistory::initialize();
		Random::initialize();
		InputSystem::getInstance()->addListener(this);
		ViewportManager::initialize();
		GameObjectManager::initialize();
		BaseComponentSystem::initialize();
		CameraManager::initialize();
		UIManager::initialize(m_windowHandle);
		
	}
	catch (...)
	{
		m_isRunning = false;
	}

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	// Initialize the Swap Chain
	RECT windowRect = this->getClientWindowRect();

	FLOAT width = windowRect.right - windowRect.left;
	FLOAT height = windowRect.bottom - windowRect.top;

	this->swapChain = renderSystem->createSwapChain(this->m_windowHandle, width, height);

	// Initialize the Constant Buffer
	CBEditor cbData;
	cbData.wireframe = false;

	this->constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBEditor));

	// Initialize Rasterizer States
	this->solidState = renderSystem->createRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_BACK);
	this->wireframeState = renderSystem->createRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);

	GDEngine::Logger::log(this, "Initialized Engine");
}

void AppWindow::draw(int width, int height, EFillMode fillMode)
{
	DeviceContext* context = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

	CBEditor cbData;
	switch (fillMode)
	{
	default:
		context->setRasterizerState(solidState);
		cbData.wireframe = false;
		break;
	case WIREFRAME:
		context->setRasterizerState(wireframeState);
		cbData.wireframe = true;
		break;
	case SOLID_WIREFRAME:
		context->setRasterizerState(solidState);
		this->draw(width, height, SOLID);
		context->setRasterizerState(wireframeState);
		cbData.wireframe = true;
		break;
	}

	context->setConstantBuffer(constantBuffer, 2);
	this->constantBuffer->update(context, &cbData);

	GameObjectManager::getInstance()->draw(width, height);
}

void AppWindow::update()
{
}

SwapChain* AppWindow::getSwapChain()
{
	return this->swapChain;
}

AppWindow* AppWindow::P_SHARED_INSTANCE = NULL;
AppWindow::AppWindow()
{
	GDEngine::Logger::log(this, "Initialized");
}
AppWindow::~AppWindow() {}
AppWindow::AppWindow(const AppWindow&) {}

AppWindow* AppWindow::getInstance() {
	return P_SHARED_INSTANCE;
}

void AppWindow::initialize()
{
	if (P_SHARED_INSTANCE)
		GDEngine::Logger::throw_exception("App Window already created");
	P_SHARED_INSTANCE = new AppWindow();
	
}

void AppWindow::destroy()
{
	if (P_SHARED_INSTANCE != NULL)
	{
		delete P_SHARED_INSTANCE->constantBuffer;
		GDEngine::Logger::log(P_SHARED_INSTANCE, "Released");
	}
}
