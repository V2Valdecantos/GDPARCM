#include "CameraManager.h"

#include "GameObjectManager.h"
#include "Logger.h"
#include "SceneCamera.h"

using namespace GDEngine;

CameraManager* CameraManager::P_SHARED_INSTANCE = NULL;

Matrix4x4 CameraManager::getMainCameraViewMatrix()
{
	return this->mainCamera->getViewMatrix();
}

Matrix4x4 CameraManager::getMainCameraProjMatrix()
{
	return this->mainCamera->getProjMatrix();
}

Matrix4x4 CameraManager::getSceneCameraViewMatrix()
{
	return this->selectedSceneCamera->getViewMatrix();
}

Matrix4x4 CameraManager::getSceneCameraProjMatrix()
{
	return this->selectedSceneCamera->getProjMatrix();
}

SceneCamera* CameraManager::getSceneCamera()
{
	return this->selectedSceneCamera;
}

SceneCamera* CameraManager::getSceneCameraByIndex(int index)
{
	if (index > sceneCameraList.size())
	{
		Logger::log(this, "Camera Index over Cameras in system");
		return NULL;
	}
	return this->sceneCameraList[index];
}

std::vector<SceneCamera*> CameraManager::getSceneCameras()
{
	return this->sceneCameraList;
}

void CameraManager::setMainCamera(Camera* camera)
{
	mainCamera = camera;
}

void CameraManager::setMainCameraByIndex(int index)
{
	if (index > cameraList.size())
	{
		Logger::log(this, "Camera Index over Cameras in system");
		return;
	}
	setMainCamera(cameraList[index]);
}

void CameraManager::setSceneCameraProjection(int type)
{
	this->selectedSceneCamera->setProjectionType(type);
}

void CameraManager::updateSceneCamera(float deltaTime)
{
	this->selectedSceneCamera->update(deltaTime);
}

void CameraManager::addCamera(Camera* camera)
{
	if (mainCamera == NULL)
		setMainCamera(camera);
	this->cameraList.push_back(camera);
}

void CameraManager::addSceneCamera(SceneCamera* camera)
{
	if (this->selectedSceneCamera == NULL)
		this->selectedSceneCamera = camera;

	this->sceneCameraList.push_back(camera);
}

void CameraManager::removeSceneCamera(SceneCamera* camera)
{
	std::string name = camera->getName();
	int index = -1;

	for (int i = 0; i < this->sceneCameraList.size() && index == -1; i++)
	{
		if (this->sceneCameraList[i] == camera)
			index = i;
	}

	if (index != -1)
	{
		this->sceneCameraList.erase(this->sceneCameraList.begin() + index);
	}
}

void CameraManager::removeCamera(Camera* camera)
{
	std::string name = camera->getName();
	int index = -1;

	for (int i = 0; i < this->cameraList.size() && index == -1; i++)
	{
		if (this->cameraList[i] == camera)
			index = i;
	}

	if (index != -1)
	{
		this->cameraList.erase(this->cameraList.begin() + index);
	}
}

CameraManager::CameraManager()
{
	this->selectedSceneCamera = new SceneCamera("Scene Camera");
	this->selectedSceneCamera->setPosition(0, 1, -8);
	this->selectedSceneCamera->updateViewMatrix();
	this->addCamera(this->selectedSceneCamera);
	Logger::log(this, "Initialized");
}

CameraManager::~CameraManager()
{
	P_SHARED_INSTANCE = nullptr;
	Logger::log(this, "Released");
}

CameraManager::CameraManager(const CameraManager&) {}

CameraManager* CameraManager::getInstance() {
	return P_SHARED_INSTANCE;
}

void CameraManager::initialize()
{
	if (P_SHARED_INSTANCE)
		Logger::throw_exception("Camera manager already created");
	P_SHARED_INSTANCE = new CameraManager();
}

void CameraManager::destroy()
{
	if (P_SHARED_INSTANCE != NULL)
	{
		delete P_SHARED_INSTANCE;
	}
}
